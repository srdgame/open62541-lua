//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "IOThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TOpcUA_IOThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TOpcUA_IOThread::TOpcUA_IOThread(UA_Client* client)
	: TThread(true), _client(client), _state(0), _oldConnectStatus(0) // always create suspended
{
	InitializeCriticalSection(&_cs);
}
//---------------------------------------------------------------------------
void __fastcall TOpcUA_IOThread::Execute()
{
	NameThreadForDebugging(System::String(L"OpcUA_IOThread"));
	//---- Place thread code here ----
	try {
		while (!Terminated) {
			StateMachine();
		}
	}
	catch (Exception& e) {

	}
	catch (...) {

	}
}
void TOpcUA_IOThread::ThreadSleep(DWORD ms)
{
	DWORD dtTick = GetTickCount();
	while (GetTickCount() - dtTick < ms) {
		Sleep(10);
		if (Terminated) return;
	}
}

void TOpcUA_IOThread::clientStateChangeTrampoline(UA_Client* client,
	UA_SecureChannelState channelState,
	UA_SessionState sessionState,
	UA_StatusCode connectStatus)
{
	UA_ClientConfig* cc = UA_Client_getConfig(client);
	TOpcUA_IOThread* pThread = (TOpcUA_IOThread*)cc->clientContext;
	if (pThread->clientStateCallback) {
		// TODO: hier auch den client->Config->clientContext korrigieren, sonst
        //       wird das über crashen!
		pThread->clientStateCallback(client, channelState, sessionState, connectStatus);
	}
}

void TOpcUA_IOThread::clientStateCallback(UA_Client *client, UA_SecureChannelState channelState,
						  UA_SessionState sessionState, UA_StatusCode connectStatus)
{
	if (_state == 30) {
		// We are in cyclic IO. Check for disconnects.
//		printf("State change cb: chn=%08Xh, ses=%08Xh, con=%08Xh\n", channelState, sessionState, connectStatus);
	}
	if (_origUserConfig.stateCallback) {
		// Call user handler, too (if any)
		_origUserConfig.stateCallback(client, channelState, sessionState, connectStatus);
	}
}
void TOpcUA_IOThread::StateMachine()
{
	UA_StatusCode retval;

	switch(_state) {
	case 0:  // Idle. Wait until we are initialized.
		break;

	case 10: { // "Connecting": Initialized, try to setup and start.
		// Do a blocking connect.
		retval = UA_Client_connect(_client, _url.c_str());
		if(retval != UA_STATUSCODE_GOOD) {
			ThreadSleep(10000);      // retry connecting every 10 seconds
		}
		else {
			_state = 20;
		}
	}
	break;

	case 20: // Connected, reading node IDs
		// get the write node info
		retval = initCyclicInfo(_wr);
		if (UA_STATUSCODE_GOOD != retval) {
			// Some error occurred.
			_state = 99;
			break;
		}
		// get the read node info
		retval = initCyclicInfo(_rd);
		if (UA_STATUSCODE_GOOD != retval) {
			// Some error occurred.
			_state = 99;
			break;
		}
		_state = 30;
//		// init write value
		_varWr = *(UA_ByteString*)_wr.varInitVal.data;
		break;

	case 30: {
		// Do the cyclic IO. Ignore any errors for now.
		_tLastRW = GetTickCount();
		retval = readwriteCyclic();
        DWORD tIO = GetTickCount();

		// Warten, bis ein Zyklus durch *UND* OPC-UA Zustandsmaschine pollen
		DWORD tElapsed = GetTickCount() - _tLastRW;
		while (GetTickCount() - _tLastRW < (_tCycleMs - 5))
		{
			UA_StatusCode connectStatus = UA_Client_run_iterate(_client, 5);
			if (_oldConnectStatus != connectStatus) {
	//			printf("Connect status : %08Xh --> %08Xh\n", _oldConnectStatus, connectStatus);
				_oldConnectStatus = connectStatus;
			}
			// Check connect status.
			if (UA_STATUSCODE_GOOD != connectStatus) {
				// Some error occurred.
				_state = 99;
				break;
			}
		}
/*
		DWORD waitInterval = 0;
		if (tElapsed < _tCycleMs) {
			waitInterval = _tCycleMs - tElapsed;
		}
		UA_StatusCode connectStatus = UA_Client_run_iterate(_client, waitInterval);
		if (_oldConnectStatus != connectStatus) {
//			printf("Connect status : %08Xh --> %08Xh\n", _oldConnectStatus, connectStatus);
			_oldConnectStatus = connectStatus;
		}
		static DWORD dwCycles = 0;
        dwCycles++;
		DWORD tDelayTime = GetTickCount() - _tLastRW;
		int iDelta = tDelayTime - _tCycleMs;
		if (iDelta < -3 || iDelta > 3) {
			printf("Jitter: %2d/%3d (%d)\n", iDelta, tIO-_tLastRW, dwCycles);
		}
*/
	}
	break;

	case 99: // Some error occurred. Disconnect and retry later.
		UA_Client_disconnect(_client);
		ThreadSleep(10000);      // retry connecting every 10 seconds
		_state = 10;
		break;

	}
}
//---------------------------------------------------------------------------
bool TOpcUA_IOThread::IsCyclicIoRunning()
{
    return (_state == 30);
}

//---------------------------------------------------------------------------
// NOTE: the returned value is a copy and *MUST be deallocated!
// i.e.
// - call UA_ByteString_init(newValue) *BEFORE* calling this function
// - call UA_ByteString_clear(newValue) *AFTER* calling this function
UA_StatusCode TOpcUA_IOThread::SetOutputs(const UA_ByteString *newValue)
{
	EnterCriticalSection(&_cs);
	UA_ByteString_copy(newValue, &_varWr);
	LeaveCriticalSection(&_cs);
	return _wrStatus;               // return the last write status
}
// NOTE: the returned value is a copy and *MUST be deallocated!
// i.e.
// - call UA_ByteString_init(newValue) *BEFORE* calling this function
// - call UA_ByteString_clear(newValue) *AFTER* calling this function
UA_StatusCode TOpcUA_IOThread::GetInputs(UA_ByteString *newValue)
{
	EnterCriticalSection(&_cs);
	UA_ByteString_copy(&_varRd, newValue);
	LeaveCriticalSection(&_cs);
	return _rdStatus;               // return the last read status
}
//---------------------------------------------------------------------------
UA_StatusCode TOpcUA_IOThread::readwriteCyclic()
{
	UA_ByteString tmp;
	UA_StatusCode retval;

	// write:
	if (_varWr.length > 0) {
		// Copy
		EnterCriticalSection(&_cs);
		UA_ByteString_init(&tmp);
		UA_ByteString_copy(&_varWr, &tmp);
		LeaveCriticalSection(&_cs);
		// Now write:
		if (_wr.Encoding.length() > 0) {
			// CoDeSys RasPi hack:
			retval = writeExtensionObjectValue(_wr.nidNodeId, _wr.nidEncoding, &tmp);
		} else {
			// Default: use data type
			retval = writeExtensionObjectValue(_wr.nidNodeId, _wr.nidDataType, &tmp);
		}
		// Clean the temporary variable
		UA_ByteString_clear(&tmp);
		_wrStatus = retval;
		if (UA_STATUSCODE_GOOD != retval) {
			return retval;
		}
	}

	// read:
	UA_Variant var;
	UA_Variant_init(&var);
	retval = TOpcUA_IOThread::readExtensionObjectValue(_rd.nidNodeId, &var);
	if (UA_STATUSCODE_GOOD == retval) {
		// check
		if (UA_Variant_isScalar(&var) && (var.type == &UA_TYPES[UA_TYPES_STRING] || var.type == &UA_TYPES[UA_TYPES_BYTESTRING])) {
			// copy
			EnterCriticalSection(&_cs);
			UA_ByteString_clear(&_varRd);
			tmp = *(UA_ByteString*)var.data;
			UA_ByteString_copy(&tmp, &_varRd);
			LeaveCriticalSection(&_cs);
		}
		else {
			retval = UA_STATUSCODE_BADENCODINGERROR; //RETURN_ERROR("not string type")
		}
	}
	_rdStatus = retval;
	UA_Variant_clear(&var);
	return retval;
}

//---------------------------------------------------------------------------
// WARNING: the returned object *must* be freed after use!
UA_StatusCode TOpcUA_IOThread::readExtensionObjectValue(const UA_NodeId nodeId, UA_Variant *outValue)
{
	// Similar to UA_Client_readValueAttribute, but returns the "raw" (undecoded)
	// data of an extension object as binary string.
	// This then allows LUA to decode the blob (e.g. using luastruct).
	// See:
	// - https://github.com/open62541/open62541/issues/3108
	// - https://github.com/open62541/open62541/issues/3787
	// - https://github.com/open62541/open62541/tree/master/examples/custom_datatype
	UA_ReadValueId item;
	UA_ReadValueId_init(&item);
	item.nodeId = nodeId;
	item.attributeId = UA_ATTRIBUTEID_VALUE;

	UA_ReadRequest request;
	UA_ReadRequest_init(&request);
	request.nodesToRead = &item;
	request.nodesToReadSize = 1;
	UA_ReadResponse response = UA_Client_Service_read(_client, request);
	UA_StatusCode retval = response.responseHeader.serviceResult;
	if (retval == UA_STATUSCODE_GOOD) {
		if(response.resultsSize == 1)
			retval = response.results[0].status;
		else
			retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
	}
	if(retval != UA_STATUSCODE_GOOD) {
		UA_ReadResponse_clear(&response);
		return retval;
	}

	/* Set the StatusCode */
	UA_DataValue *res = response.results;
	if (res->hasStatus)
		retval = res->status;

	/* Return early of no value is given */
	if (!res->hasValue) {
		retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
		UA_ReadResponse_clear(&response);
		return retval;
	}

	/* Copy value into out */
	if (res->value.type->typeKind == UA_DATATYPEKIND_EXTENSIONOBJECT) {
		UA_ExtensionObject* eo = (UA_ExtensionObject*)(res->value.data);
		//UA_ExtensionObjectEncoding encoding = eo->encoding;
		//UA_NodeId nodeId = eo->content.encoded.typeId;
		int length = eo->content.encoded.body.length;
		UA_Byte* data = eo->content.encoded.body.data;
		UA_Variant_setScalarCopy(outValue, &eo->content.encoded.body,  &UA_TYPES[UA_TYPES_BYTESTRING]);
		//UA_Variant_init(&res->value);
	}
	else {
		UA_Variant_init(outValue);
		retval = UA_STATUSCODE_BADTYPEMISMATCH;
		UA_ReadResponse_clear(&response);
		return retval;
	}

	UA_ReadResponse_clear(&response);
	return retval;
}


UA_StatusCode TOpcUA_IOThread::writeExtensionObjectValue(const UA_NodeId nodeId, const UA_NodeId& dataTypeNodeId, const UA_ByteString *newValue)
{
	// Similar to UA_Client_writeValueAttribute, but returns the "raw" (undecoded)
	// data of an extension object as binary string.
	// This then allows LUA to decode the blob (e.g. using luastruct).
	// See:
	// - https://github.com/open62541/open62541/issues/3108
	// - https://github.com/open62541/open62541/issues/3787
	// - https://github.com/open62541/open62541/tree/master/examples/custom_datatype
	// - https://groups.google.com/g/open62541/c/DIrQsGDQ8k4
	UA_Variant myVariant; /* Variants can hold scalar values and arrays of any type */
	UA_Variant_init(&myVariant);
	// build the extension object
	UA_ExtensionObject dataValue;
	dataValue.encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
	dataValue.content.encoded.typeId = dataTypeNodeId;
	dataValue.content.encoded.body.data = (UA_Byte*)newValue->data;
	dataValue.content.encoded.body.length = newValue->length;

	UA_Variant_setScalarCopy(&myVariant, &dataValue, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
	if (myVariant.type == &UA_TYPES[UA_TYPES_STRING]) {
		// enforce bytestring (if not yet)
		myVariant.type = &UA_TYPES[UA_TYPES_BYTESTRING];
	}
	UA_StatusCode retval = UA_Client_writeValueAttribute(_client, nodeId, &myVariant);
	UA_Variant_clear(&myVariant);
	return retval;
}


// Do the initial transactions to get all information we need after connecting
UA_StatusCode TOpcUA_IOThread::initCyclicInfo(TOpcUA_IOThread::CyclicNode& cycNode)
{
	//UA_NodeClass outNodeClass;
	UA_NodeId nodeId;

    // Clear everything to prevent memory leaks for multiple calls.
	UA_NodeId_clear(&cycNode.nidNodeId);
	UA_NodeId_clear(&cycNode.nidDataType);
	UA_NodeId_clear(&cycNode.nidEncoding);
	UA_NodeClass_clear(&cycNode.nidNodeClass);
	UA_Variant_clear(&cycNode.varInitVal);

	nodeId = UA_NODEID_STRING_ALLOC(cycNode.Namespace, cycNode.Name.c_str());
	UA_StatusCode retval = UA_Client_readNodeIdAttribute(_client, nodeId, &cycNode.nidNodeId);
	if (UA_STATUSCODE_GOOD == retval) {
		// return UA_Client_readDataTypeAttribute(_client, nodeId, outDataType);
		retval = UA_Client_readDataTypeAttribute(_client, cycNode.nidNodeId, &cycNode.nidDataType);
		if (UA_STATUSCODE_GOOD == retval) {
			retval = UA_Client_readNodeClassAttribute(_client, cycNode.nidNodeId, &cycNode.nidNodeClass);
			if (UA_STATUSCODE_GOOD == retval) {
				UA_Variant_init(&cycNode.varInitVal);
				retval = readExtensionObjectValue(cycNode.nidNodeId, &cycNode.varInitVal);
			}
		}
	}
	UA_NodeId_clear(&nodeId);

	if (UA_STATUSCODE_GOOD == retval && cycNode.Encoding.length() > 0) {
		nodeId = UA_NODEID_STRING_ALLOC(cycNode.Namespace, cycNode.Encoding.c_str());
		UA_StatusCode retval = UA_Client_readNodeIdAttribute(_client, nodeId, &cycNode.nidEncoding);
		UA_NodeId_clear(&nodeId);
	}

	return retval;
}
//---------------------------------------------------------------------------
void TOpcUA_IOThread::Init(
	const char* endpoint_url,   // "opc.tcp://10.10.2.27:4840"
	int ns,         		// namespace
	const char* wrNode,     // node name to use for writing to OPC UA server
	const char* wrEncoding,
	const char* rdNode,     // node name to use for reading from OPC UA server
	const char* rdEncoding,
	DWORD cycleMs,          // read/write cycle time
	const char* username,
	const char* password)
{
	if (_state != 0) {
		// Error!
		return;
	}
	_url            = endpoint_url;
	_user           = username;
	_pass           = password;
	_tCycleMs       = cycleMs;

	_wr.Init(ns, wrNode, wrEncoding);
	_rd.Init(ns, rdNode, rdEncoding);

	// We also get the client configuration and modify it to link our
	// local callbacks
	UA_ClientConfig *cc = UA_Client_getConfig(_client);
	_origUserConfig = *cc;
	// Wire up the callbacks
	cc->clientContext = this;
	cc->stateCallback = &TOpcUA_IOThread::clientStateChangeTrampoline;
	//cc->subscriptionInactivityCallback = &UA_Client_Proxy::subscriptionInactivityCallback;

	_state = 10;
	Resume();
}
//---------------------------------------------------------------------------
