#pragma once
#include <iostream>

#include "open62541.h"
#include "examples/common.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_interfaces.hpp"
#include "opcua_node.hpp"
//#include "certificates.h"

namespace lua_opcua {

#define CLINET_SERVICES_REQUEST(XT, XN) \
UA_##XT##Response XN(const UA_##XT##Request request) { \
	return UA_Client_Service_##XN(_client, request); \
} \

class ClientService {
		UA_Client* _client;
	public:
		ClientService(UA_Client* client) : _client(client) {}
		CLINET_SERVICES_REQUEST(Read, read)
		CLINET_SERVICES_REQUEST(Write, write)
		CLINET_SERVICES_REQUEST(Call, call)
		CLINET_SERVICES_REQUEST(AddNodes, addNodes)
		CLINET_SERVICES_REQUEST(AddReferences, addReferences)
		CLINET_SERVICES_REQUEST(DeleteNodes, deleteNodes)
		CLINET_SERVICES_REQUEST(DeleteReferences, deleteReferences)
		CLINET_SERVICES_REQUEST(Browse, browse)
		CLINET_SERVICES_REQUEST(BrowseNext, browseNext)
		CLINET_SERVICES_REQUEST(TranslateBrowsePathsToNodeIds, translateBrowsePathsToNodeIds)
		CLINET_SERVICES_REQUEST(RegisterNodes, registerNodes)
		CLINET_SERVICES_REQUEST(UnregisterNodes, unregisterNodes)
		//CLINET_SERVICES_REQUEST(QueryFirst, queryFirst)
		//CLINET_SERVICES_REQUEST(QueryNext, queryNext)
};

class ClientAttributeReader : public AttributeReader {
	UA_Client* _client;
public:
	ClientAttributeReader(UA_Client* client) : _client(client) {}
	UA_StatusCode readNodeId(const UA_NodeId nodeId, UA_NodeId *outNodeId) {
		return UA_Client_readNodeIdAttribute(_client, nodeId, outNodeId);
	}
	UA_StatusCode readNodeClass(const UA_NodeId nodeId, UA_NodeClass *outNodeClass) {
		return UA_Client_readNodeClassAttribute(_client, nodeId, outNodeClass);
	}
	UA_StatusCode readBrowseName(const UA_NodeId nodeId, UA_QualifiedName *outBrowseName) {
		return UA_Client_readBrowseNameAttribute(_client, nodeId, outBrowseName);
	}
	UA_StatusCode readDisplayName(const UA_NodeId nodeId, UA_LocalizedText *outDisplayName) {
		return UA_Client_readDisplayNameAttribute(_client, nodeId, outDisplayName);
	}
	UA_StatusCode readDescription(const UA_NodeId nodeId, UA_LocalizedText *outDescription) {
		return UA_Client_readDescriptionAttribute(_client, nodeId, outDescription);
	}
	UA_StatusCode readWriteMask(const UA_NodeId nodeId, UA_UInt32 *outWriteMask) {
		return UA_Client_readWriteMaskAttribute(_client, nodeId, outWriteMask);
	}
	UA_StatusCode readUserWriteMask(const UA_NodeId nodeId, UA_UInt32 *outUserWriteMask) {
		return UA_Client_readUserWriteMaskAttribute(_client, nodeId, outUserWriteMask);
	}
	UA_StatusCode readIsAbstract(const UA_NodeId nodeId, UA_Boolean *outIsAbstract) {
		return UA_Client_readIsAbstractAttribute(_client, nodeId, outIsAbstract);
	}
	UA_StatusCode readSymmetric(const UA_NodeId nodeId, UA_Boolean *outSymmetric) {
		return UA_Client_readSymmetricAttribute(_client, nodeId, outSymmetric);
	}
	UA_StatusCode readInverseName(const UA_NodeId nodeId, UA_LocalizedText *outInverseName) {
		return UA_Client_readInverseNameAttribute(_client, nodeId, outInverseName);
	}
	UA_StatusCode readContainsNoLoops(const UA_NodeId nodeId, UA_Boolean *outContainsNoLoops) {
		return UA_Client_readContainsNoLoopsAttribute(_client, nodeId, outContainsNoLoops);
	}
	UA_StatusCode readEventNotifier(const UA_NodeId nodeId, UA_Byte *outEventNotifier) {
		return UA_Client_readEventNotifierAttribute(_client, nodeId, outEventNotifier);
	}
	UA_StatusCode readValue(const UA_NodeId nodeId, UA_Variant *outValue) {
		return UA_Client_readValueAttribute(_client, nodeId, outValue);
	}
	UA_StatusCode readDataValue(const UA_NodeId nodeId, UA_DataValue *outDataValue) {
		UA_ReadValueId id; UA_ReadValueId_init(&id);
		id.nodeId = nodeId;
		id.attributeId = UA_ATTRIBUTEID_VALUE;
		*outDataValue = UA_Client_readAttribute(_client, &id);
		return outDataValue->status;
	}
	UA_StatusCode readDataType(const UA_NodeId nodeId, UA_NodeId *outDataType) {
		return UA_Client_readDataTypeAttribute(_client, nodeId, outDataType);
	}
	UA_StatusCode readValueRank(const UA_NodeId nodeId, UA_Int32 *outValueRank) {
		return UA_Client_readValueRankAttribute(_client, nodeId, outValueRank);
	}
	UA_StatusCode readArrayDimensions(const UA_NodeId nodeId, size_t *outArrayDimensionsSize, UA_UInt32 **outArrayDimensions) {
		return UA_Client_readArrayDimensionsAttribute(_client, nodeId, outArrayDimensionsSize, outArrayDimensions);
	}
	UA_StatusCode readAccessLevel(const UA_NodeId nodeId, UA_Byte *outAccessLevel) {
		return UA_Client_readAccessLevelAttribute(_client, nodeId, outAccessLevel);
	}
	UA_StatusCode readUserAccessLevel(const UA_NodeId nodeId, UA_Byte *outUserAccessLevel) {
		return UA_Client_readUserAccessLevelAttribute(_client, nodeId, outUserAccessLevel);
	}
	UA_StatusCode readMinimumSamplingInterval(const UA_NodeId nodeId, UA_Double *outMinSamplingInterval) {
		return UA_Client_readMinimumSamplingIntervalAttribute(_client, nodeId, outMinSamplingInterval);
	}
	UA_StatusCode readHistorizing(const UA_NodeId nodeId, UA_Boolean *outHistorizing) {
		return UA_Client_readHistorizingAttribute(_client, nodeId, outHistorizing);
	}
	UA_StatusCode readExecutable(const UA_NodeId nodeId, UA_Boolean *outExecutable) {
		return UA_Client_readExecutableAttribute(_client, nodeId, outExecutable);
	}
	UA_StatusCode readUserExecutable(const UA_NodeId nodeId, UA_Boolean *outUserExecutable) {
		return UA_Client_readUserExecutableAttribute(_client, nodeId, outUserExecutable);
	}
};

class ClientAttributeWriter : public AttributeWriter {
	UA_Client* _client;
public:
	ClientAttributeWriter(UA_Client* client) : _client(client) {}
	UA_StatusCode writeNodeId(const UA_NodeId nodeId, const UA_NodeId *newNodeId) {
		return UA_Client_writeNodeIdAttribute(_client, nodeId, newNodeId);
	}
	UA_StatusCode writeNodeClass(const UA_NodeId nodeId, const UA_NodeClass *newNodeClass) {
		return UA_Client_writeNodeClassAttribute(_client, nodeId, newNodeClass);
	}
	UA_StatusCode writeBrowseName(const UA_NodeId nodeId, const UA_QualifiedName *newBrowseName) {
		return UA_Client_writeBrowseNameAttribute(_client, nodeId, newBrowseName);
	}
	UA_StatusCode writeDisplayName(const UA_NodeId nodeId, const UA_LocalizedText *newDisplayName) {
		return UA_Client_writeDisplayNameAttribute(_client, nodeId, newDisplayName);
	}
	UA_StatusCode writeDescription(const UA_NodeId nodeId, const UA_LocalizedText *newDescription) {
		return UA_Client_writeDescriptionAttribute(_client, nodeId, newDescription);
	}
	UA_StatusCode writeWriteMask(const UA_NodeId nodeId, const UA_UInt32 *newWriteMask) {
		return UA_Client_writeWriteMaskAttribute(_client, nodeId, newWriteMask);
	}
	UA_StatusCode writeUserWriteMask(const UA_NodeId nodeId, const UA_UInt32 *newUserWriteMask) {
		return UA_Client_writeUserWriteMaskAttribute(_client, nodeId, newUserWriteMask);
	}
	UA_StatusCode writeIsAbstract(const UA_NodeId nodeId, const UA_Boolean *newIsAbstract) {
		return UA_Client_writeIsAbstractAttribute(_client, nodeId, newIsAbstract);
	}
	UA_StatusCode writeSymmetric(const UA_NodeId nodeId, const UA_Boolean *newSymmetric) {
		return UA_Client_writeSymmetricAttribute(_client, nodeId, newSymmetric);
	}
	UA_StatusCode writeInverseName(const UA_NodeId nodeId, const UA_LocalizedText *newInverseName)  {
		return UA_Client_writeInverseNameAttribute(_client, nodeId, newInverseName);
	}
	UA_StatusCode writeContainsNoLoops(const UA_NodeId nodeId, const UA_Boolean *newContainsNoLoops) {
		return UA_Client_writeContainsNoLoopsAttribute(_client, nodeId, newContainsNoLoops);
	}
	UA_StatusCode writeEventNotifier(const UA_NodeId nodeId, const UA_Byte *newEventNotifier) {
		return UA_Client_writeEventNotifierAttribute(_client, nodeId, newEventNotifier);
	}
	UA_StatusCode writeValue(const UA_NodeId nodeId, const UA_Variant *newValue) {
		return UA_Client_writeValueAttribute(_client, nodeId, newValue);
	}
	UA_StatusCode writeDataValue(const UA_NodeId nodeId, const UA_DataValue *newDataValue) {
		UA_WriteValue val; UA_WriteValue_init(&val);
		val.nodeId = nodeId;
		val.attributeId = UA_ATTRIBUTEID_VALUE;
		val.value = *newDataValue;
		return UA_Client_writeAttribute(_client, &val);
	}
	UA_StatusCode writeDataType(const UA_NodeId nodeId, const UA_NodeId *newDataType) {
		return UA_Client_writeDataTypeAttribute(_client, nodeId, newDataType);
	}
	UA_StatusCode writeValueRank(const UA_NodeId nodeId, const UA_Int32 *newValueRank) {
		return UA_Client_writeValueRankAttribute(_client, nodeId, newValueRank);
	}
	UA_StatusCode writeArrayDimensions(const UA_NodeId nodeId, size_t newArrayDimensionsSize, const UA_UInt32 *newArrayDimensions) {
		return UA_Client_writeArrayDimensionsAttribute(_client, nodeId, newArrayDimensionsSize, newArrayDimensions);
	}
	UA_StatusCode writeAccessLevel(const UA_NodeId nodeId, const UA_Byte *newAccessLevel) {
		return UA_Client_writeAccessLevelAttribute(_client, nodeId, newAccessLevel);
	}
	UA_StatusCode writeUserAccessLevel(const UA_NodeId nodeId, const UA_Byte *newUserAccessLevel) {
		return UA_Client_writeUserAccessLevelAttribute(_client, nodeId, newUserAccessLevel);
	}
	UA_StatusCode writeMinimumSamplingInterval(const UA_NodeId nodeId, const UA_Double *newMinInterval) {
		return UA_Client_writeMinimumSamplingIntervalAttribute(_client, nodeId, newMinInterval);
	}
	UA_StatusCode writeHistorizing(const UA_NodeId nodeId, const UA_Boolean *newHistorizing) {
		return UA_Client_writeHistorizingAttribute(_client, nodeId, newHistorizing);
	}
	UA_StatusCode writeExecutable(const UA_NodeId nodeId, const UA_Boolean *newExecutable) {
		return UA_Client_writeExecutableAttribute(_client, nodeId, newExecutable);
	}
	UA_StatusCode writeUserExecutable(const UA_NodeId nodeId, const UA_Boolean *newUserExecutable) {
		return UA_Client_writeUserExecutableAttribute(_client, nodeId, newUserExecutable);
	}
};

class ClientNodeMgr : public NodeMgr {
	UA_Client* _client;
	ClientAttributeReader _reader;
	ClientAttributeWriter _writer;
public:
	ClientNodeMgr(UA_Client* client) : _client(client), _reader(client), _writer(client) {}
	AttributeReader* getAttributeReader() {
		return &_reader;
	}
	AttributeWriter* getAttributeWriter() {
		return &_writer;
	}
	UA_StatusCode addReference(const UA_NodeId sourceNodeId,
			const UA_NodeId referenceTypeId,
			const UA_ExpandedNodeId targetNodeId,
			UA_Boolean isForward,
			const UA_String targetServerUri,
			UA_NodeClass targetNodeClass) {
		return UA_Client_addReference(_client, sourceNodeId, referenceTypeId, isForward, targetServerUri, targetNodeId, targetNodeClass);
	}
	UA_StatusCode deleteReference(const UA_NodeId sourceNodeId,
			const UA_NodeId referenceTypeId,
			const UA_ExpandedNodeId targetNodeId,
			UA_Boolean isForward,
			UA_NodeClass targetNodeClass) {
		return UA_Client_deleteReference(_client, sourceNodeId, referenceTypeId, isForward, targetNodeId, targetNodeClass);
	}
	UA_StatusCode deleteNode(const UA_NodeId nodeId, bool deleteTargetReferences) {
		return UA_Client_deleteNode(_client, nodeId, deleteTargetReferences);
	}
	UA_StatusCode addVariable(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addVariableNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, typeDefinition, attr, outNewNodeId);
	}
	UA_StatusCode addVariableType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_VariableTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addVariableTypeNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, outNewNodeId);
	}
	UA_StatusCode addObject(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_ObjectAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addObjectNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, typeDefinition, attr, outNewNodeId);
	}
	UA_StatusCode addObjectType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ObjectTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addObjectTypeNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, outNewNodeId);
	}
	UA_StatusCode addView(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ViewAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addViewNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, outNewNodeId);
	}
	UA_StatusCode addReferenceType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ReferenceTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addReferenceTypeNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, outNewNodeId);
	}
	UA_StatusCode addDataType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_DataTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addDataTypeNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, outNewNodeId);
	}
	UA_StatusCode addMethod(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_MethodAttributes attr,
			UA_MethodCallback method,
			size_t inputArgumentsSize, const UA_Argument* inputArguments,
			size_t outputArgumentsSize, const UA_Argument* outputArguments,
			void *nodeContext,
			UA_NodeId *outNewNodeId) {
		return UA_Client_addMethodNode(_client, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, outNewNodeId);
	}
	UA_StatusCode callMethod(const UA_NodeId objectId,
			const UA_NodeId methodId,
			size_t inputSize,
			const UA_Variant *input,
            size_t *outputSize,
			UA_Variant **output) {
		return UA_Client_call(_client, objectId, methodId, inputSize, input, outputSize, output);
	}

	UA_StatusCode forEachChildNodeCall(UA_NodeId parentNodeId, 
			UA_NodeIteratorCallback callback,
			void *handle) {
		return UA_Client_forEachChildNodeCall(_client, parentNodeId, callback, handle);
	}
};

/* cleanupClient deletes the memory allocated for client configuration.
 *
 * @param  client             client configuration that need to be deleted
 * @param  remoteCertificate  server certificate */
static void cleanupClient(UA_Client* client, UA_ByteString* remoteCertificate) {
    UA_ByteString_delete(remoteCertificate); /* Dereference the memory */
    UA_Client_delete(client); /* Disconnects the client internally */
}


void UA_LUA_Logger(UA_LogLevel level, UA_LogCategory category, const char *msg, va_list args);
class UA_Client_Proxy {
	UA_Client_Proxy(UA_Client_Proxy& prox);
public:
	UA_Client* _client;
	ClientNodeMgr* _mgr;
	std::string _endpoint_url;
	UA_Client_Proxy(const char* endpointUrl, UA_UInt32 timeout, UA_UInt32 secureChannelLifeTime, UA_ConnectionConfig connectionConfig) {
		UA_ClientConfig config = {
			timeout, // 5000
			secureChannelLifeTime, //10 * 60 * 1000	
			//UA_Log_Stdout,
			UA_LUA_Logger,
			connectionConfig, /*
								 {
								 0,
								 65535,
								 65535,
								 0,
								 0,
								 }
								 */
			UA_ClientConnectionTCP,
			0,
			NULL
		};
		_endpoint_url = std::string(endpointUrl);
		_client = UA_Client_new(config);
		_mgr = new ClientNodeMgr(_client);
	}
	UA_Client_Proxy(const char* endpointUrl, UA_UInt32 timeout, UA_UInt32 secureChannelLifeTime, UA_ConnectionConfig connectionConfig,
			const char* securityPolicy, const char* certificate, const char* privateKey, const char* remoteCertificate) {
		UA_ClientConfig config = {
			timeout, // 5000
			secureChannelLifeTime, //10 * 60 * 1000	
			//UA_Log_Stdout,
			UA_LUA_Logger,
			connectionConfig, /*
								 {
								 0,
								 65535,
								 65535,
								 0,
								 0,
								 }
								 */
			UA_ClientConnectionTCP,
			0,
			NULL
		};

		_endpoint_url = std::string(endpointUrl);

		if (securityPolicy && strlen(securityPolicy) > 0) {
			if (remoteCertificate && strlen(remoteCertificate) > 0) {
				_client = initSecureClient(securityPolicy, certificate, privateKey, remoteCertificate);
			} else {
				_client = initSecureClient(securityPolicy, certificate, privateKey);
			}
		} else {
			_client = UA_Client_new(config);
		}

		if (!_client) {
			printf("Initialized UA_Client failure!!!");
			exit(-1);
		}

		_mgr = new ClientNodeMgr(_client);
	}
	~UA_Client_Proxy() {
		UA_Client_delete(_client);
		delete _mgr;
	}

	UA_Client* initSecureClient(const char* securityPolicy, const char* priCert, const char* priKey, const char* remoteCert) {
		/* Load certificate and private key */
		UA_ByteString           certificate        = loadFile(priCert);
		UA_ByteString           privateKey         = loadFile(priKey);
		UA_ByteString			remoteCertificate  = loadFile(remoteCert);

		UA_Client*              client             = NULL;

		UA_SecurityPolicy_Func sp_func = NULL;
		if (strcmp(securityPolicy, "Basic128Rsa15") == 0) {
			sp_func = UA_SecurityPolicy_Basic128Rsa15;
		} else if(strcmp(securityPolicy, "Basic256Sha256") == 0) {
			sp_func = UA_SecurityPolicy_Basic256Sha256;
		} 

		/* Secure client initialization */
		client = UA_Client_secure_new(UA_ClientConfig_default,
				certificate, privateKey,
				&remoteCertificate,
				NULL, 0,
				NULL, 0,
				sp_func);
		
		UA_ByteString_deleteMembers(&remoteCertificate);
		UA_ByteString_deleteMembers(&certificate);
		UA_ByteString_deleteMembers(&privateKey);

		return client;	
	}

	UA_Client* initSecureClient(const char* securityPolicy, const char* priCert, const char* priKey) {
		UA_Client*              client             = NULL;
		UA_ByteString*          remoteCertificate  = NULL;
		UA_StatusCode           retval             = UA_STATUSCODE_GOOD;
		size_t                  trustListSize      = 0;
		UA_ByteString*          revocationList     = NULL;
		size_t                  revocationListSize = 0;

		/* endpointArray is used to hold the available endpoints in the server
		 * endpointArraySize is used to hold the number of endpoints available */
		UA_EndpointDescription* endpointArray      = NULL;
		size_t                  endpointArraySize  = 0;

		UA_SecurityPolicy_Func sp_func = NULL;
		if (strcmp(securityPolicy, "Basic128Rsa15") == 0) {
			sp_func = UA_SecurityPolicy_Basic128Rsa15;
		} else if(strcmp(securityPolicy, "Basic256Sha256") == 0) {
			sp_func = UA_SecurityPolicy_Basic256Sha256;
		} 

		/* The Get endpoint (discovery service) is done with
		 * security mode as none to see the server's capability
		 * and certificate */
		client = UA_Client_new(UA_ClientConfig_default);
		remoteCertificate = UA_ByteString_new();
		retval = UA_Client_getEndpoints(client, _endpoint_url.c_str(),
				&endpointArraySize, &endpointArray);
		if(retval != UA_STATUSCODE_GOOD) {
			UA_Array_delete(endpointArray, endpointArraySize,
					&UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
			cleanupClient(client, remoteCertificate);
			//return (int)retval;
			return NULL;
		}

		char* temp = new char[256];
		sprintf(temp, "http://opcfoundation.org/UA/SecurityPolicy#%s", securityPolicy);
		UA_String securityPolicyUri = UA_STRING(temp);

		printf("%i endpoints found\n", (int)endpointArraySize);
		for(size_t endPointCount = 0; endPointCount < endpointArraySize; endPointCount++) {
			printf("URL of endpoint %i is %.*s / %.*s\n", (int)endPointCount,
					(int)endpointArray[endPointCount].endpointUrl.length,
					endpointArray[endPointCount].endpointUrl.data,
					(int)endpointArray[endPointCount].securityPolicyUri.length,
					endpointArray[endPointCount].securityPolicyUri.data);
			printf("securityMode %d - %d\n", endpointArray[endPointCount].securityMode, UA_MESSAGESECURITYMODE_SIGNANDENCRYPT);

			if(endpointArray[endPointCount].securityMode != UA_MESSAGESECURITYMODE_SIGNANDENCRYPT)
				continue;

			if(UA_String_equal(&endpointArray[endPointCount].securityPolicyUri, &securityPolicyUri)) {
				UA_ByteString_copy(&endpointArray[endPointCount].serverCertificate, remoteCertificate);
				break;
			}
		}
		delete[] temp;

		if(UA_ByteString_equal(remoteCertificate, &UA_BYTESTRING_NULL)) {
			UA_LOG_FATAL(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"Server does not support Security %s Mode of"
					" UA_MESSAGESECURITYMODE_SIGNANDENCRYPT", securityPolicy);
			cleanupClient(client, remoteCertificate);
			return NULL;
		}

		UA_Array_delete(endpointArray, endpointArraySize,
				&UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

		UA_Client_delete(client); /* Disconnects the client internally */

		/* Load the trustList. Load revocationList is not supported now */
		/*
		if(argc > MIN_ARGS)
			trustListSize = (size_t)argc-MIN_ARGS;

		UA_STACKARRAY(UA_ByteString, trustList, trustListSize);
		for(size_t trustListCount = 0; trustListCount < trustListSize; trustListCount++) {
			trustList[trustListCount] = loadFile(argv[trustListCount+3]);
		}
		*/
		UA_ByteString trustList[1];

		/* Load certificate and private key */
		UA_ByteString           certificate        = loadFile(priCert);
		UA_ByteString           privateKey         = loadFile(priKey);
		/*
		UA_ByteString certificate;
		certificate.length = CERT_DER_LENGTH;
		certificate.data = CERT_DER_DATA;

		UA_ByteString privateKey;
		privateKey.length = KEY_DER_LENGTH;
		privateKey.data = KEY_DER_DATA;
		*/

		/* Secure client initialization */
		client = UA_Client_secure_new(UA_ClientConfig_default,
				certificate, privateKey,
				remoteCertificate,
				trustList, trustListSize,
				revocationList, revocationListSize,
				sp_func);
		if(client == NULL) {
			UA_ByteString_delete(remoteCertificate); /* Dereference the memory */
		}

		UA_ByteString_deleteMembers(&certificate);
		UA_ByteString_deleteMembers(&privateKey);
		for(size_t deleteCount = 0; deleteCount < trustListSize; deleteCount++) {
			UA_ByteString_deleteMembers(&trustList[deleteCount]);
		}


		return client;
	}

	UA_ClientState getState() {
		return UA_Client_getState(_client);
	}
	void reset() {
		UA_Client_reset(_client);
	}
	UA_StatusCode connect() {
		return UA_Client_connect(_client, _endpoint_url.c_str());
	}
	UA_StatusCode connect_username(const char* username, const char* password) {
		return UA_Client_connect_username(_client, _endpoint_url.c_str(), username, password);
	}
	UA_StatusCode disconnect() {
		return UA_Client_disconnect(_client);
	}
	UA_StatusCode manuallyRenewSecureChannel() {
		return UA_Client_manuallyRenewSecureChannel(_client);
	}
	UA_StatusCode getEndpoints(const char* serverUrl, size_t* endpointDescriptionsSize, UA_EndpointDescription** endpointDescriptions) {
		return UA_Client_getEndpoints(_client, serverUrl, endpointDescriptionsSize, endpointDescriptions);
	}
	UA_StatusCode findServers(const char* serverUrl, size_t serverUrisSize, UA_String* serverUris, size_t localeIdsSize, UA_String* localeIds, size_t* registeredServersSize, UA_ApplicationDescription **registeredServers) {
		return UA_Client_findServers(_client, serverUrl, serverUrisSize, serverUris, localeIdsSize, localeIds, registeredServersSize, registeredServers);
	}
	//UA_StatusCode findServersOnNetwork(

	sol::variadic_results getNamespaceIndex(const char* namespaceUri, sol::this_state L) {
		UA_UInt16 namespaceIndex = -1;
		UA_String uri = UA_STRING_ALLOC(namespaceUri);
		UA_StatusCode re = UA_Client_NamespaceGetIndex(_client, &uri, &namespaceIndex);
		UA_free(uri.data);
		RETURN_RESULT(UA_UInt16, namespaceIndex)
	}
	ClientNodeMgr* getNodeMgr() {
		return _mgr;
	}
	UA_Node getObjectsNode() {
		return UA_Node(_mgr, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_NODECLASS_OBJECT);
	}
	UA_Node getTypesNode() {
		return UA_Node(_mgr, UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_NODECLASS_OBJECT);
	}
	UA_Node getViewsNode() {
		return UA_Node(_mgr, UA_NODEID_NUMERIC(0, UA_NS0ID_VIEWSFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_NODECLASS_OBJECT);
	}
	UA_Node getServerNode() {
		return UA_Node(_mgr, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_NODECLASS_OBJECT);
	}
	UA_Node getRootNode() {
		return UA_Node(_mgr, UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_NODECLASS_OBJECT);
	}
	sol::variadic_results getNode(const UA_NodeId& id, sol::this_state L) {
		UA_NodeId outId;
		UA_NodeClass outNodeClass;
		auto reader = _mgr->getAttributeReader();
		UA_StatusCode re = reader->readNodeId(id, &outId);
		if (re == UA_STATUSCODE_GOOD) {
			//std::cout << "readNodeClass: " << UA_StatusCode_name(reader->readNodeClass(outId, &outNodeClass)) << std::endl;
			reader->readNodeClass(outId, &outNodeClass);
		}
		RETURN_RESULT(UA_Node, UA_Node(_mgr, outId, UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), outNodeClass));
	}
	sol::variadic_results getNode(int ns, int id, sol::this_state L) {
		return getNode(UA_NODEID_NUMERIC(ns, id), L);
	}
	sol::variadic_results getNode(int ns, const char* id, sol::this_state L) {
		return getNode(UA_NODEID_STRING(ns, (char*)id), L);
	}
	sol::variadic_results getNode(int ns, const UA_Guid& id, sol::this_state L) {
		return getNode(UA_NODEID_GUID(ns, id), L);
	}

	UA_StatusCode deleteNode(const UA_NodeId& nodeId, bool deleteReferences) {
		return _mgr->deleteNode(nodeId, deleteReferences);
	}
	UA_StatusCode deleteNode(const UA_Node& node, bool deleteReferences) {
		return deleteNode(node._id, deleteReferences);
	}
};

void reg_opcua_client(sol::table& module) {
	module.new_usertype<UA_ConnectionConfig>("ConnectionConfig",
		"protocolVersion", &UA_ConnectionConfig::protocolVersion,
		"sendBufferSize", &UA_ConnectionConfig::sendBufferSize,
		"recvBufferSize", &UA_ConnectionConfig::recvBufferSize,
		"maxMessageSize", &UA_ConnectionConfig::maxMessageSize,
		"maxChunkCount", &UA_ConnectionConfig::maxChunkCount
	);
	module.new_usertype<UA_ClientConfig>("ClientConfig",
		"timeout", &UA_ClientConfig::timeout,
		"secureChannelLifeTime", &UA_ClientConfig::secureChannelLifeTime,
		//"logger", &UA_ClientConfig::logger,
		"localConnectionConfig", &UA_ClientConfig::localConnectionConfig
		//"connectionFunc", &UA_ClientConfig::connectionFunc,
		//"customDataTypesSize", &UA_ClientConfig::customDataTypesSize
	);

	module.new_usertype<UA_Client_Proxy>("Client",
		//sol::constructors<UA_Client_Proxy(UA_UInt32, UA_UInt32, UA_ConnectionConfig)>(),
		sol::constructors<UA_Client_Proxy(const char*, UA_UInt32, UA_UInt32, UA_ConnectionConfig), UA_Client_Proxy(const char*, UA_UInt32, UA_UInt32, UA_ConnectionConfig, const char*, const char*, const char*, const char*)>(),
		"getState", &UA_Client_Proxy::getState,
		"reset", &UA_Client_Proxy::reset,
		"connect", &UA_Client_Proxy::connect,
		"connect_username", &UA_Client_Proxy::connect_username,
		"disconnect", &UA_Client_Proxy::disconnect,
		"manuallyRenewSecureChannel", &UA_Client_Proxy::manuallyRenewSecureChannel,
		"getEndpoints", &UA_Client_Proxy::getEndpoints,
		"findServers", &UA_Client_Proxy::findServers,
		"getNamespaceIndex", &UA_Client_Proxy::getNamespaceIndex,
		"getNodeMgr", &UA_Client_Proxy::getNodeMgr,
		"getObjectsNode", &UA_Client_Proxy::getObjectsNode,
		"getTypesNode", &UA_Client_Proxy::getTypesNode,
		"getViewsNode", &UA_Client_Proxy::getViewsNode,
		"getServerNode", &UA_Client_Proxy::getServerNode,
		"getRootNode", &UA_Client_Proxy::getRootNode,
		"getNode", sol::overload(
			static_cast<sol::variadic_results (UA_Client_Proxy::*)(const UA_NodeId&, sol::this_state L) >(&UA_Client_Proxy::getNode),
			static_cast<sol::variadic_results (UA_Client_Proxy::*)(int, int, sol::this_state L) >(&UA_Client_Proxy::getNode),
			static_cast<sol::variadic_results (UA_Client_Proxy::*)(int, const char*, sol::this_state L) >(&UA_Client_Proxy::getNode),
			static_cast<sol::variadic_results (UA_Client_Proxy::*)(int, const UA_Guid&, sol::this_state L) >(&UA_Client_Proxy::getNode)
		),
		"deleteNode", sol::overload(
			static_cast<UA_StatusCode (UA_Client_Proxy::*)(const UA_NodeId&, bool) >(&UA_Client_Proxy::deleteNode),
			static_cast<UA_StatusCode (UA_Client_Proxy::*)(const UA_Node&, bool) >(&UA_Client_Proxy::deleteNode)
		)
	);

	module.new_usertype<ClientNodeMgr>("ClientNodeMgr",
		//sol::constructors<ClientNodeMgr(UA_Client*)>(),
		"new", sol::no_constructor,
		"addReference", &ClientNodeMgr::addReference,
		"deleteReference", &ClientNodeMgr::deleteReference,
		"deleteNode", &ClientNodeMgr::deleteNode,
		"addObject", &ClientNodeMgr::addObject,
		"addObjectType", &ClientNodeMgr::addObjectType,
		"addVariable", &ClientNodeMgr::addVariable,
		"addVariableType", &ClientNodeMgr::addVariableType,
		"addReferenceType", &ClientNodeMgr::addReferenceType,
		"addDataType", &ClientNodeMgr::addDataType
		//"addMethod", &ClientNodeMgr::addMethod
	);

}

}
