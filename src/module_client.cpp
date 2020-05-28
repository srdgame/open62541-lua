#include <iostream>
#include <map>
#include <list>

#include "open62541.h"
#include "read_file.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_interfaces.hpp"
#include "module_node.hpp"
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
			// UA_MethodCallback method,
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

class UA_ClientConfig_Proxy {
	UA_ClientConfig *_config;
protected:
	friend class UA_Client_Proxy;

	UA_ClientConfig_Proxy(UA_ClientConfig *config) : _config(config) {
	}
public:
	void setTimeout(int timeout) {
		_config->timeout = timeout;
	}
	void setSecureChannelLifeTime(int time) {
		_config->secureChannelLifeTime = time;
	}
	void setProductURI(const std::string& uri) {
		/*
		UA_String_clear(&_config->buildInfo.productUri);
		_config->buildInfo.productUri = UA_STRING_ALLOC((char*)uri.c_str())
		*/
		UA_String_clear(&_config->clientDescription.productUri);
		_config->clientDescription.productUri = UA_STRING_ALLOC((char*)uri.c_str());
	}
	void setApplicationURI(const std::string& uri) {
		UA_String_clear(&_config->clientDescription.applicationUri);
		_config->clientDescription.applicationUri = UA_STRING_ALLOC((char*)uri.c_str());
	}
	void setApplicationName(const std::string& locale, const std::string& name) {
		UA_LocalizedText_clear(&_config->clientDescription.applicationName);
		_config->clientDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC(locale.c_str(), name.c_str());
	}
};

class UA_Client_Proxy {
protected:
	UA_Client_Proxy(UA_Client_Proxy& prox);
	UA_Client* _client;
	ClientNodeMgr* _mgr;

	typedef std::function<void(UA_UInt32 monId, UA_DataValue value, UA_UInt32 subId, void *monContext)> SubscribeCallback;
	std::map<UA_UInt32, SubscribeCallback> _subCallbackMap;

	/*
	struct SubscribeCallbackItem {
		UA_UInt32 sub_id;
		UA_UInt32 mon_id;
		UA_DataValue value;
		void* context;
		SubscribeCallbackItem(UA_UInt32 sub_id, UA_UInt32 mon_id, UA_DataValue* value) 
			: sub_id(sub_id),
			mon_id(mon_id)
		{
			UA_DataValue_copy(value, &this->value);
		}
	};
	std::list<SubscribeCallbackItem*> _subCallbackItems;
	*/

	typedef std::function<void(UA_Client_Proxy* client,
			UA_SecureChannelState channelState,
			UA_SessionState sessionState,
			UA_StatusCode connectStatus)> StateCallback;
	StateCallback _stateCallback;
	//UA_ClientState _stateCallbackNew;
	

	static void
		handler_MonitoredItemChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
				UA_UInt32 monId, void *monContext, UA_DataValue *value) {
			UA_Client_Proxy* pClient = (UA_Client_Proxy*)subContext;
			pClient->handleMonitoredItemChange(client, subId, monId, monContext, value);
		}

	static void
		handler_MonitoredItemDeleted(UA_Client *client, UA_UInt32 subId, void *subContext,
				UA_UInt32 monId, void *monContext) {
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Monitored item %u was deleted, sub_id %u", monId, subId);
		}

	static void
		clientStateChangeCallback(UA_Client* client,
			UA_SecureChannelState channelState,
			UA_SessionState sessionState,
			UA_StatusCode connectStatus) {

			UA_ClientConfig* cc = UA_Client_getConfig(client);
			UA_Client_Proxy* pClient = (UA_Client_Proxy*)cc->clientContext;
			if (pClient->_stateCallback) {
				pClient->_stateCallback(pClient, channelState, sessionState, connectStatus);
			}
		}

	static void
		subscriptionInactivityCallback (UA_Client *client, UA_UInt32 subId, void *subContext) {
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Inactivity for subscription %u", subId);
		}

	static void
		statusChangeSubscriptionCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
				UA_StatusChangeNotification *notification) {
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Subscription Id %u status was changed", subId);
		}

	static void
		deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext) {
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Subscription Id %u was deleted", subscriptionId);
		}
public:
	UA_ClientConfig_Proxy *_config;

	UA_Client_Proxy() {
		_stateCallback = nullptr;
		//_stateCallbackNew = (UA_ClientState)-1;
		_client = UA_Client_new();
		if (!_client) {
			printf("Initialized UA_Client failure!!!");
			exit(-1);
		}
		UA_ClientConfig* cc = UA_Client_getConfig(_client);
		UA_ClientConfig_setDefault(cc);

		cc->clientContext = this;
		cc->stateCallback = &UA_Client_Proxy::clientStateChangeCallback;
		cc->subscriptionInactivityCallback = &UA_Client_Proxy::subscriptionInactivityCallback;

		_mgr = new ClientNodeMgr(_client);
		_config = new UA_ClientConfig_Proxy(cc);
	}

	UA_Client_Proxy(UA_MessageSecurityMode securityMode, const std::string& priCert, const std::string& priKey) {
		_stateCallback = nullptr;
		//_stateCallbackNew = (UA_ClientState)-1;
		/* Load certificate and private key */
		UA_ByteString certificate = loadFile(priCert.c_str());
		UA_ByteString privateKey  = loadFile(priKey.c_str());

		/* Load the trustList. Load revocationList is not supported now */
		/* trust list not support for now
		const size_t trustListSize = 0;
		if(argc > MIN_ARGS)
			trustListSize = (size_t)argc-MIN_ARGS;
		UA_STACKARRAY(UA_ByteString, trustList, trustListSize);
		for(size_t trustListCount = 0; trustListCount < trustListSize; trustListCount++)
			trustList[trustListCount] = loadFile(argv[trustListCount+4]);
		*/

		UA_ByteString *trustList = NULL;
		size_t trustListSize = 0;

		UA_ByteString *revocationList = NULL;
		size_t revocationListSize = 0;

		_client = UA_Client_new();
		UA_ClientConfig *cc = UA_Client_getConfig(_client);
		cc->securityMode = securityMode;
		UA_StatusCode rc = UA_ClientConfig_setDefaultEncryption(cc, certificate, privateKey,
				trustList, trustListSize,
				revocationList, revocationListSize);

		UA_ByteString_clear(&certificate);
		UA_ByteString_clear(&privateKey);

		/*
		for(size_t deleteCount = 0; deleteCount < trustListSize; deleteCount++) {
			UA_ByteString_clear(&trustList[deleteCount]);
		}
		*/

		/* Secure client connect */
		cc->securityMode = securityMode; /* require encryption */

		cc->clientContext = this;
		cc->stateCallback = &UA_Client_Proxy::clientStateChangeCallback;
		cc->subscriptionInactivityCallback = &UA_Client_Proxy::subscriptionInactivityCallback;

		_mgr = new ClientNodeMgr(_client);
		_config = new UA_ClientConfig_Proxy(cc);
	}

	~UA_Client_Proxy() {
		UA_Client_delete(_client);
		delete _mgr;
	}

	void setStateCallback(StateCallback callback) {
		_stateCallback = callback;
	}

	/*
	UA_ClientState getState() {
		return UA_Client_getState(_client);
	}
	*/

	sol::variadic_results getState(sol::this_state L) {
		UA_SecureChannelState chn_s;
		UA_SessionState ss_s;
		UA_StatusCode sc;
		UA_Client_getState(_client, &chn_s, &ss_s, &sc);

		sol::variadic_results result;
		result.push_back({ L, sol::in_place_type<UA_SecureChannelState>, chn_s});
		result.push_back({ L, sol::in_place_type<UA_SessionState>, ss_s});
		result.push_back({ L, sol::in_place_type<UA_StatusCode>, sc});

		return result;
	}
	/*
	void reset() {
		UA_Client_reset(_client);
	}
	*/
	UA_StatusCode connect(const char* endpoint_url) {
		return UA_Client_connect(_client, endpoint_url);
	}
	UA_StatusCode connect_username(const char* endpoint_url, const char* username, const char* password) {
		//return UA_Client_connect_username(_client, endpoint_url, username, password);
		return UA_Client_connectUsername(_client, endpoint_url, username, password);
	}
	UA_StatusCode connectUsername(char* endpoint_url, const char* username, const char* password) {
		return UA_Client_connectUsername(_client, endpoint_url, username, password);
	}
	UA_StatusCode disconnect() {
		return UA_Client_disconnect(_client);
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

	sol::variadic_results createSubscription(SubscribeCallback callback, sol::this_state L) {
		UA_StatusCode re = -1;
		if (!this->_client) {
			RETURN_RESULT(UA_UInt32, -1);
		}
		/* A new session was created. We need to create the subscription. */
		/* Create a subscription */
		UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
		UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(this->_client, request,
				this, statusChangeSubscriptionCallback, deleteSubscriptionCallback);

		re = response.responseHeader.serviceResult;
		if ( re == UA_STATUSCODE_GOOD) {
			_subCallbackMap[response.subscriptionId] = callback;
		}
		RETURN_RESULT(UA_UInt32, response.subscriptionId);
	}
	void handleMonitoredItemChange(UA_Client *client, UA_UInt32 subId, UA_UInt32 monId, void *monContext, UA_DataValue *value) {
		if (_client != client) {
			return;
		}
		/*
		SubscribeCallbackItem* item = new SubscribeCallbackItem(subId, monId, value);
		_subCallbackItems.push_back(item);
		*/

		UA_DataValue val;
		UA_DataValue_copy(value, &val);
		auto ptr = _subCallbackMap.find(subId);
		if (ptr != _subCallbackMap.end()) {
			(ptr->second)(monId, val, subId, monContext);
		} else {
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Cannot find %u was deleted", subId);
		}
	}

	sol::variadic_results subscribeNode(UA_UInt32 sub_id, const UA_NodeId& nodeId, sol::this_state L) {
		/* Add a MonitoredItem */
		UA_MonitoredItemCreateRequest monRequest =
			UA_MonitoredItemCreateRequest_default(nodeId);

		UA_MonitoredItemCreateResult monResponse =
			UA_Client_MonitoredItems_createDataChange(this->_client, sub_id,
					UA_TIMESTAMPSTORETURN_BOTH, monRequest, NULL,
					handler_MonitoredItemChanged, handler_MonitoredItemDeleted);

		UA_StatusCode re = monResponse.statusCode;
		RETURN_RESULT(UA_UInt32, monResponse.monitoredItemId)
	}

	sol::variadic_results callMethod(const UA_NodeId& objectId, const UA_NodeId& methodId, sol::variadic_args args, sol::this_state L) {
		size_t inputSize = args.size();
		size_t outputSize = 0;
		UA_Variant *inputs = new UA_Variant[inputSize];
		UA_Variant *outputs;
		sol::variadic_results result;

		// Copy the inputs
		for (int i = 0; i < inputSize; ++i) {
			UA_Variant_init(inputs + i);
			UA_Variant* v = args.get<UA_Variant*>(i);
			UA_Variant_copy(v, inputs + i);
		}

		/// Call object method
		UA_StatusCode re = _mgr->callMethod(objectId, methodId, inputSize, inputs, &outputSize, &outputs);

		// Free input array
		for (int i = 0; i < inputSize; ++i) {
			UA_Variant_clear(inputs + i);
		}
		delete [] inputs;

		// Check the returns
		if (re == UA_STATUSCODE_GOOD) {
			// Push the output size
			result.push_back({ L, sol::in_place_type<size_t>, outputSize});
			// Push all output items
			for (int i = 0; i < outputSize; ++i) {
				UA_Variant var;
				UA_Variant_copy(outputs + i, &var);
				result.push_back({ L, sol::in_place_type<UA_Variant>, var});
			}
		} else {
			// Push the error message
			result.push_back({ L, sol::in_place_type<sol::lua_nil_t>, sol::lua_nil_t()});
			result.push_back({ L, sol::in_place_type<std::string>, std::string(UA_StatusCode_name(re))});
		}

		// Free the outputs array which is contrustured inside the library.
		UA_Array_delete(outputs, outputSize, &UA_TYPES[UA_TYPES_VARIANT]);

		return result;
	}



	UA_UInt16 run_iterate(UA_UInt32 waitInternal) {
		/*
		UA_UInt32 last = 0;
		UA_StatusCode re;
		do {
			re = UA_Client_run_iterate(this->_client, 20);
			if (_subCallbackItems.size() > 0) {
				auto ptr = _subCallbackItems.begin();
				for (; ptr != _subCallbackItems.end(); ++ptr) {
					SubscribeCallbackItem& item = **ptr;

					auto mptr = _subCallbackMap.find(item.sub_id);
					if (mptr != _subCallbackMap.end()) {
						UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Trigger calback", item.sub_id);
						(mptr->second)(item.mon_id, &item.value, item.sub_id, item.context);
						break;
					} else {
						UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Cannot find %u callback", item.sub_id);
					}

					delete *ptr;
				}
				_subCallbackItems.clear();
			}
			if (_stateCallbackNew != -1 && _stateCallback) {
				_stateCallback(this, _stateCallbackNew);
				_stateCallbackNew = (UA_ClientState)-1;
			}

		} while ( ( (last + 20) < waitInternal) && re == UA_STATUSCODE_GOOD);

		return re;
		*/
		return UA_Client_run_iterate(this->_client, waitInternal);
	}
	
};


void reg_opcua_client(sol::table& module) {
	module.new_usertype<UA_ConnectionConfig>("ConnectionConfig",
		"protocolVersion", &UA_ConnectionConfig::protocolVersion,
		"sendBufferSize", &UA_ConnectionConfig::sendBufferSize,
		"recvBufferSize", &UA_ConnectionConfig::recvBufferSize,
		"localMaxMessageSize", &UA_ConnectionConfig::localMaxMessageSize,
		"remoteMaxMessageSize", &UA_ConnectionConfig::remoteMaxMessageSize,
		"localMaxChunkCount", &UA_ConnectionConfig::localMaxChunkCount,
		"remoteMaxChunkCount", &UA_ConnectionConfig::remoteMaxChunkCount
	);
	module.new_usertype<UA_ClientConfig_Proxy>("ClientConfig",
		"new", sol::no_constructor, 
		"setTimeout", &UA_ClientConfig_Proxy::setTimeout,
		"setSecureChannelLifeTime", &UA_ClientConfig_Proxy::setSecureChannelLifeTime,
		"setProductURI", &UA_ClientConfig_Proxy::setProductURI,
		"setApplicationURI", &UA_ClientConfig_Proxy::setApplicationURI,
		"setApplicationName", &UA_ClientConfig_Proxy::setApplicationName
	);
	/*
	module.new_usertype<UA_ApplicationDescription>("ApplicationDescription",
		"applicationUri", &UA_ApplicationDescription::applicationUri,
		"productUri", &UA_ApplicationDescription::productUri,
		"applicationName", &UA_ApplicationDescription::applicationName,
		"applicationType", &UA_ApplicationDescription::applicationType,
		"gatewayServerUri", &UA_ApplicationDescription::gatewayServerUri,
		"discoveryProfileUri", &UA_ApplicationDescription::discoveryProfileUri,
		"discoveryUrlsSize", &UA_ApplicationDescription::discoveryUrlsSize,
		"discoveryUrls", &UA_ApplicationDescription::discoveryUrls
	);
	*/
	module.new_usertype<UA_ClientConfig>("ClientConfig",
		"timeout", &UA_ClientConfig::timeout,
		"clientDescription", &UA_ClientConfig::clientDescription,

		"secureChannelLifeTime", &UA_ClientConfig::secureChannelLifeTime,
		"requestedSessionTimeout", &UA_ClientConfig::requestedSessionTimeout,
		"localConnectionConfig", &UA_ClientConfig::localConnectionConfig,
		"connectivityCheckInterval", &UA_ClientConfig::connectivityCheckInterval
	);

	module.new_usertype<UA_Client_Proxy>("Client",
		sol::constructors<UA_Client_Proxy(), UA_Client_Proxy(UA_MessageSecurityMode, const std::string&, const std::string&)>(),
		"config", &UA_Client_Proxy::_config,
		"setStateCallback", &UA_Client_Proxy::setStateCallback,
		"getState", &UA_Client_Proxy::getState,
		//"reset", &UA_Client_Proxy::reset,
		"connect", &UA_Client_Proxy::connect,
		"connect_username", &UA_Client_Proxy::connect_username,
		"connectUsername", &UA_Client_Proxy::connectUsername,
		"disconnect", &UA_Client_Proxy::disconnect,
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
		),
		"createSubscription", &UA_Client_Proxy::createSubscription,
		"subscribeNode", &UA_Client_Proxy::subscribeNode,
		"callMethod", &UA_Client_Proxy::callMethod,
		"run_iterate", &UA_Client_Proxy::run_iterate
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
