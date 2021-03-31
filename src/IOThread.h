//---------------------------------------------------------------------------

#ifndef IOThreadH
#define IOThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include <open62541.h>
#include <string>
//---------------------------------------------------------------------------
class TOpcUA_IOThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall TOpcUA_IOThread(UA_Client* client);
	void Init(
		const char* endpoint_url,
		int ns,         		// namespace
		const char* wrNode,     // node name to use for writing to OPC UA server
		const char* wrEncoding,
		const char* rdNode,     // node name to use for reading from OPC UA server
		const char* rdEncoding,
		DWORD cycleMs,          // read/write cycle time
		const char* username = "",
		const char* password = ""
	);
	bool IsCyclicIoRunning();
	UA_StatusCode SetOutputs(const UA_ByteString *newValue);
	UA_StatusCode GetInputs(UA_ByteString *newValue);

private:
	class CyclicNode {
	public:
		CyclicNode() {
			UA_NodeId_init(&nidNodeId);
			UA_NodeId_init(&nidDataType);
			UA_NodeId_init(&nidEncoding);
			UA_NodeClass_init(&nidNodeClass);
			UA_Variant_init(&varInitVal);
		}
		~CyclicNode() {
			UA_NodeId_clear(&nidNodeId);
			UA_NodeId_clear(&nidDataType);
			UA_NodeId_clear(&nidEncoding);
			UA_NodeClass_clear(&nidNodeClass);
			UA_Variant_clear(&varInitVal);
		}
		void Init(int ns, const char* name, const char* encoding = "") {
			Namespace = ns;
			Name = name;
			Encoding = encoding;
		}
		int                 Namespace;
		std::string         Name;
		std::string         Encoding;
		UA_NodeId 			nidNodeId;          // the variable nodeID (to read/write)
		UA_NodeId 			nidDataType;        // the data type nodeID
		UA_NodeId 			nidEncoding;        // the encoding nodeID
		UA_NodeClass 		nidNodeClass;       // the variable nodeID (to read/write)
		UA_Variant          varInitVal;         // initial value (read to get size of extension objects)
	};
	UA_Client* 			_client;
	CRITICAL_SECTION 	_cs;
	int                 _state;
	std::string         _url, _user, _pass;
	DWORD               _tCycleMs, _tLastRW;
	CyclicNode          _wr, _rd;
	UA_ByteString       _varWr, _varRd;
	UA_StatusCode       _wrStatus, _rdStatus, _oldConnectStatus;
	void StateMachine();
	void ThreadSleep(DWORD ms);
	UA_StatusCode readExtensionObjectValue(const UA_NodeId nodeId, UA_Variant *outValue);
	UA_StatusCode writeExtensionObjectValue(const UA_NodeId nodeId, const UA_NodeId& dataTypeNodeId, const UA_ByteString *newValue);
	UA_StatusCode initCyclicInfo(TOpcUA_IOThread::CyclicNode& cycNode);
	UA_StatusCode readwriteCyclic();
	UA_ClientConfig 	_origUserConfig;
	static void clientStateChangeTrampoline(
		UA_Client* client,
		UA_SecureChannelState channelState,
		UA_SessionState sessionState,
		UA_StatusCode connectStatus);
	void clientStateCallback(UA_Client *client, UA_SecureChannelState channelState,
						  UA_SessionState sessionState, UA_StatusCode connectStatus);
};
//---------------------------------------------------------------------------
#endif
