#include <iostream>
#include <list>

#include "open62541.h"
#include "read_file.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_interfaces.hpp"
#include "module_node.hpp"

namespace lua_opcua {

struct UA_DataSource_Proxy {
	UA_DataSource _ds;

	typedef std::function<UA_StatusCode (UA_DataSource_Proxy &proxy, const UA_NodeId* sessionId,
										void *sesssionContext, const UA_NodeId* nodeId, 
										UA_Boolean includeSourceTimeStamp, const UA_NumericRange *range,
										UA_DataValue* value) > OnReadCallback;

	typedef std::function<UA_StatusCode (UA_DataSource_Proxy &proxy, const UA_NodeId* sessionId,
										void *sesssionContext, const UA_NodeId* nodeId, 
										const UA_NumericRange *range,
										const UA_DataValue* value) > OnWriteCallback;

	OnReadCallback _read;
	OnWriteCallback _write;

	static UA_StatusCode ReadCallback(UA_Server *server, const UA_NodeId *sessionId,
                          void *sessionContext, const UA_NodeId *nodeId,
                          void *nodeContext, UA_Boolean includeSourceTimeStamp,
                          const UA_NumericRange *range, UA_DataValue *value) {
		UA_DataSource_Proxy* p = (UA_DataSource_Proxy*)nodeContext;
		if (p->_read) {
			return p->_read(*p, sessionId, sessionContext, nodeId, includeSourceTimeStamp, range, value);
		} else {
			return UA_STATUSCODE_BADINTERNALERROR;
		}
	}

	static UA_StatusCode WriteCallback(UA_Server *server, const UA_NodeId *sessionId,
                           void *sessionContext, const UA_NodeId *nodeId,
                           void *nodeContext, const UA_NumericRange *range,
                           const UA_DataValue *value) {
		UA_DataSource_Proxy* p = (UA_DataSource_Proxy*)nodeContext;
		if (p->_write) {
			return p->_write(*p, sessionId, sessionContext, nodeId, range, value);
		} else {
			return UA_STATUSCODE_BADINTERNALERROR;
		}
	}
	UA_DataSource_Proxy(OnReadCallback onRead, OnWriteCallback onWrite) : _read(onRead), _write(onWrite) {
		_ds.read = &UA_DataSource_Proxy::ReadCallback;
		_ds.write = &UA_DataSource_Proxy::WriteCallback;
	}
	~UA_DataSource_Proxy() {
		_read = nullptr;
		_write = nullptr;
	}
};

class UA_Server_Proxy;

struct UA_ValueCallback_Proxy {
	UA_ValueCallback _callback;
	typedef std::function<void (UA_Server_Proxy *proxy, const UA_NodeId* sessionId,
										void *sesssionContext, const UA_NodeId* nodeId,
										const UA_NumericRange *range,
										const UA_DataValue* value) > OnReadCallback;

	typedef std::function<void (UA_Server_Proxy *proxy, const UA_NodeId* sessionId,
										void *sesssionContext, const UA_NodeId* nodeId,
										const UA_NumericRange *range,
										const UA_DataValue* data) > OnWriteCallback;
	OnReadCallback _read;
	OnWriteCallback _write;
	UA_Server_Proxy* _proxy;

	static void ReadCallback(UA_Server *server, const UA_NodeId *sessionId,
                   void *sessionContext, const UA_NodeId *nodeId,
                   void *nodeContext, const UA_NumericRange *range,
                   const UA_DataValue *value);

	static void WriteCallback(UA_Server *server, const UA_NodeId *sessionId,
                    void *sessionContext, const UA_NodeId *nodeId,
                    void *nodeContext, const UA_NumericRange *range,
                    const UA_DataValue *data);

	UA_ValueCallback_Proxy(OnReadCallback onRead, OnWriteCallback onWrite) : _read(onRead), _write(onWrite), _proxy(nullptr) {
		_callback.onRead = &UA_ValueCallback_Proxy::ReadCallback;
		_callback.onWrite = &UA_ValueCallback_Proxy::WriteCallback;
	}
	~UA_ValueCallback_Proxy() {
		_read = nullptr;
		_write = nullptr;
		_proxy = nullptr;
	}
};

class ServerAttributeReader : public AttributeReader {
	UA_Server* _server;
public:
	ServerAttributeReader(UA_Server* client) : _server(client) {}
	UA_StatusCode readNodeId(const UA_NodeId nodeId, UA_NodeId *outNodeId) {
		return UA_Server_readNodeId(_server, nodeId, outNodeId);
	}
	UA_StatusCode readNodeClass(const UA_NodeId nodeId, UA_NodeClass *outNodeClass) {
		return UA_Server_readNodeClass(_server, nodeId, outNodeClass);
	}
	UA_StatusCode readBrowseName(const UA_NodeId nodeId, UA_QualifiedName *outBrowseName) {
		return UA_Server_readBrowseName(_server, nodeId, outBrowseName);
	}
	UA_StatusCode readDisplayName(const UA_NodeId nodeId, UA_LocalizedText *outDisplayName) {
		return UA_Server_readDisplayName(_server, nodeId, outDisplayName);
	}
	UA_StatusCode readDescription(const UA_NodeId nodeId, UA_LocalizedText *outDescription) {
		return UA_Server_readDescription(_server, nodeId, outDescription);
	}
	UA_StatusCode readWriteMask(const UA_NodeId nodeId, UA_UInt32 *outWriteMask) {
		return UA_Server_readWriteMask(_server, nodeId, outWriteMask);
	}
	UA_StatusCode readUserWriteMask(const UA_NodeId nodeId, UA_UInt32 *outUserWriteMask) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode readIsAbstract(const UA_NodeId nodeId, UA_Boolean *outIsAbstract) {
		return UA_Server_readIsAbstract(_server, nodeId, outIsAbstract);
	}
	UA_StatusCode readSymmetric(const UA_NodeId nodeId, UA_Boolean *outSymmetric) {
		return UA_Server_readSymmetric(_server, nodeId, outSymmetric);
	}
	UA_StatusCode readInverseName(const UA_NodeId nodeId, UA_LocalizedText *outInverseName) {
		return UA_Server_readInverseName(_server, nodeId, outInverseName);
	}
	UA_StatusCode readContainsNoLoops(const UA_NodeId nodeId, UA_Boolean *outContainsNoLoops) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode readEventNotifier(const UA_NodeId nodeId, UA_Byte *outEventNotifier) {
		return UA_Server_readEventNotifier(_server, nodeId, outEventNotifier);
	}
	UA_StatusCode readValue(const UA_NodeId nodeId, UA_Variant *outValue) {
		return UA_Server_readValue(_server, nodeId, outValue);
	}
	UA_StatusCode readDataValue(const UA_NodeId nodeId, UA_DataValue *outDataValue) {
		UA_ReadValueId id; UA_ReadValueId_init(&id);
		id.nodeId = nodeId;
		id.attributeId = UA_ATTRIBUTEID_VALUE;
		*outDataValue = UA_Server_read(_server, &id, UA_TIMESTAMPSTORETURN_BOTH);
		return outDataValue->status;
	}
	UA_StatusCode readDataType(const UA_NodeId nodeId, UA_NodeId *outDataType) {
		return UA_Server_readDataType(_server, nodeId, outDataType);
	}
	UA_StatusCode readValueRank(const UA_NodeId nodeId, UA_Int32 *outValueRank) {
		return UA_Server_readValueRank(_server, nodeId, outValueRank);
	}
	UA_StatusCode readArrayDimensions(const UA_NodeId nodeId, size_t *outArrayDimensionsSize, UA_UInt32 **outArrayDimensions) {
		//return UA_Server_readArrayDimensions(_server, nodeId, outArrayDimensionsSize, outArrayDimensions);
		// TODO:
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode readAccessLevel(const UA_NodeId nodeId, UA_Byte *outAccessLevel) {
		return UA_Server_readAccessLevel(_server, nodeId, outAccessLevel);
	}
	UA_StatusCode readUserAccessLevel(const UA_NodeId nodeId, UA_Byte *outUserAccessLevel) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode readMinimumSamplingInterval(const UA_NodeId nodeId, UA_Double *outMinSamplingInterval) {
		return UA_Server_readMinimumSamplingInterval(_server, nodeId, outMinSamplingInterval);
	}
	UA_StatusCode readHistorizing(const UA_NodeId nodeId, UA_Boolean *outHistorizing) {
		return UA_Server_readHistorizing(_server, nodeId, outHistorizing);
	}
	UA_StatusCode readExecutable(const UA_NodeId nodeId, UA_Boolean *outExecutable) {
		return UA_Server_readExecutable(_server, nodeId, outExecutable);
	}
	UA_StatusCode readUserExecutable(const UA_NodeId nodeId, UA_Boolean *outUserExecutable) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
};

class ServerAttributeWriter : public AttributeWriter {
	UA_Server* _server;
public:
	ServerAttributeWriter(UA_Server* client) : _server(client) {}
	UA_StatusCode writeNodeId(const UA_NodeId nodeId, const UA_NodeId *newNodeId) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeNodeClass(const UA_NodeId nodeId, const UA_NodeClass *newNodeClass) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeBrowseName(const UA_NodeId nodeId, const UA_QualifiedName *newBrowseName) {
		return UA_Server_writeBrowseName(_server, nodeId, *newBrowseName);
	}
	UA_StatusCode writeDisplayName(const UA_NodeId nodeId, const UA_LocalizedText *newDisplayName) {
		return UA_Server_writeDisplayName(_server, nodeId, *newDisplayName);
	}
	UA_StatusCode writeDescription(const UA_NodeId nodeId, const UA_LocalizedText *newDescription) {
		return UA_Server_writeDescription(_server, nodeId, *newDescription);
	}
	UA_StatusCode writeWriteMask(const UA_NodeId nodeId, const UA_UInt32 *newWriteMask) {
		return UA_Server_writeWriteMask(_server, nodeId, *newWriteMask);
	}
	UA_StatusCode writeUserWriteMask(const UA_NodeId nodeId, const UA_UInt32 *newUserWriteMask) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeIsAbstract(const UA_NodeId nodeId, const UA_Boolean *newIsAbstract) {
		return UA_Server_writeIsAbstract(_server, nodeId, *newIsAbstract);
	}
	UA_StatusCode writeSymmetric(const UA_NodeId nodeId, const UA_Boolean *newSymmetric) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeInverseName(const UA_NodeId nodeId, const UA_LocalizedText *newInverseName)  {
		return UA_Server_writeInverseName(_server, nodeId, *newInverseName);
	}
	UA_StatusCode writeContainsNoLoops(const UA_NodeId nodeId, const UA_Boolean *newContainsNoLoops) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeEventNotifier(const UA_NodeId nodeId, const UA_Byte *newEventNotifier) {
		return UA_Server_writeEventNotifier(_server, nodeId, *newEventNotifier);
	}
	UA_StatusCode writeValue(const UA_NodeId nodeId, const UA_Variant *newValue) {
		return UA_Server_writeValue(_server, nodeId, *newValue);
	}
	UA_StatusCode writeDataValue(const UA_NodeId nodeId, const UA_DataValue *newDataValue) {
		UA_WriteValue val; UA_WriteValue_init(&val);
		val.nodeId = nodeId;
		val.attributeId = UA_ATTRIBUTEID_VALUE;
		val.value = *newDataValue;
		return UA_Server_write(_server, &val);
	}
	UA_StatusCode writeDataType(const UA_NodeId nodeId, const UA_NodeId *newDataType) {
		return UA_Server_writeDataType(_server, nodeId, *newDataType);
	}
	UA_StatusCode writeValueRank(const UA_NodeId nodeId, const UA_Int32 *newValueRank) {
		return UA_Server_writeValueRank(_server, nodeId, *newValueRank);
	}
	UA_StatusCode writeArrayDimensions(const UA_NodeId nodeId, size_t newArrayDimensionsSize, const UA_UInt32 *newArrayDimensions) {
		//return UA_Server_writeArrayDimensions(_server, nodeId, newArrayDimensionsSize, *newArrayDimensions);
		// TODO:
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeAccessLevel(const UA_NodeId nodeId, const UA_Byte *newAccessLevel) {
		return UA_Server_writeAccessLevel(_server, nodeId, *newAccessLevel);
	}
	UA_StatusCode writeUserAccessLevel(const UA_NodeId nodeId, const UA_Byte *newUserAccessLevel) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeMinimumSamplingInterval(const UA_NodeId nodeId, const UA_Double *newMinInterval) {
		return UA_Server_writeMinimumSamplingInterval(_server, nodeId, *newMinInterval);
	}
	UA_StatusCode writeHistorizing(const UA_NodeId nodeId, const UA_Boolean *newHistorizing) {
		//return UA_Server_writeHistorizing(_server, nodeId, *newHistorizing);
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
	UA_StatusCode writeExecutable(const UA_NodeId nodeId, const UA_Boolean *newExecutable) {
		return UA_Server_writeExecutable(_server, nodeId, *newExecutable);
	}
	UA_StatusCode writeUserExecutable(const UA_NodeId nodeId, const UA_Boolean *newUserExecutable) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}
};

class ServerNodeMgr : public NodeMgr {
	UA_Server* _server;
	ServerAttributeReader _reader;
	ServerAttributeWriter _writer;
public:
	ServerNodeMgr(UA_Server* client) : _server(client), _reader(client), _writer(client) {}
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
		return UA_Server_addReference(_server, sourceNodeId, referenceTypeId, targetNodeId, isForward);
	}
	UA_StatusCode deleteReference(const UA_NodeId sourceNodeId,
			const UA_NodeId referenceTypeId,
			const UA_ExpandedNodeId targetNodeId,
			UA_Boolean isForward,
			UA_NodeClass targetNodeClass) {
		return UA_Server_deleteReference(_server, sourceNodeId, referenceTypeId, isForward, targetNodeId, targetNodeClass);
	}
	UA_StatusCode deleteNode(const UA_NodeId nodeId, bool deleteTargetReferences) {
		return UA_Server_deleteNode(_server, nodeId, deleteTargetReferences);
	}
	UA_StatusCode addVariable(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addVariableNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, typeDefinition, attr, NULL, outNewNodeId);
	}
	UA_StatusCode addDataSourceVariable(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr,
			UA_DataSource_Proxy dataSource,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addDataSourceVariableNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, typeDefinition, attr, dataSource._ds, &dataSource, outNewNodeId);
	}
	UA_StatusCode addVariableType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_VariableTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addVariableTypeNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, UA_NODEID_NULL, attr, NULL, outNewNodeId);
	}
	UA_StatusCode addObject(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_ObjectAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addObjectNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, typeDefinition, attr, NULL, outNewNodeId);
	}
	UA_StatusCode addObjectType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ObjectTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addObjectTypeNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, NULL, outNewNodeId);
	}
	UA_StatusCode addView(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ViewAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addViewNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, NULL, outNewNodeId);
	}
	UA_StatusCode addReferenceType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ReferenceTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addReferenceTypeNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, NULL, outNewNodeId);
	}
	UA_StatusCode addDataType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_DataTypeAttributes attr,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addDataTypeNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, NULL, outNewNodeId);
	}
	const UA_DataType* findDataType(const UA_NodeId *typeId) {
		return UA_Server_findDataType(_server, typeId);
	}
	UA_StatusCode addMethod(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_MethodAttributes attr,
			//UA_MethodCallback method,
			size_t inputArgumentsSize, const UA_Argument* inputArguments,
			size_t outputArgumentsSize, const UA_Argument* outputArguments,
			void *nodeContext,
			UA_NodeId *outNewNodeId) {
		return UA_Server_addMethodNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, NULL, inputArgumentsSize, inputArguments, outputArgumentsSize, outputArguments, nodeContext, outNewNodeId);
	}
	UA_StatusCode callMethod(const UA_NodeId objectId,
			const UA_NodeId methodId,
			size_t inputSize,
			const UA_Variant *input,
            size_t *outputSize,
			UA_Variant **output) {
		return UA_STATUSCODE_BADNOTSUPPORTED;
	}

	UA_StatusCode forEachChildNodeCall(UA_NodeId parentNodeId, 
			UA_NodeIteratorCallback callback,
			void *handle) {
		return UA_Server_forEachChildNodeCall(_server, parentNodeId, callback, handle);
	}
};

class UA_ServerConfig_Proxy {
	UA_ServerConfig *_config;
protected:
	friend class UA_Server_Proxy;

	UA_ServerConfig_Proxy(UA_ServerConfig *config) : _config(config) {
		//printf("%lld\n", _config);
	}
public:
	void setProductURI(const std::string& uri) {
		/*
		UA_String_clear(&_config->buildInfo.productUri);
		_config->buildInfo.productUri = UA_STRING_ALLOC((char*)uri.c_str())
		*/
		UA_String_clear(&_config->applicationDescription.productUri);
		_config->applicationDescription.productUri = UA_STRING_ALLOC((char*)uri.c_str());
	}
	void setApplicationURI(const std::string& uri) {
		UA_String_clear(&_config->applicationDescription.applicationUri);
		_config->applicationDescription.applicationUri = UA_STRING_ALLOC((char*)uri.c_str());
	}
	void setApplicationName(const std::string& locale, const std::string& name) {
		UA_LocalizedText_clear(&_config->applicationDescription.applicationName);
		_config->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC(locale.c_str(), name.c_str());
	}
};

class UA_Server_Proxy {
protected:
	friend class UA_ValueCallback_Proxy;
	UA_Server_Proxy(UA_Server_Proxy& prox);
	UA_Server* _server;
	ServerNodeMgr* _mgr;

public:
	UA_ServerConfig_Proxy *_config;
	volatile bool _running = true;

public:
	UA_Server_Proxy() {
		_server = UA_Server_new();
		UA_ServerConfig *cc = UA_Server_getConfig(_server);
		UA_ServerConfig_setDefault(cc);
		_config = new UA_ServerConfig_Proxy(cc);
		_mgr = new ServerNodeMgr(_server);
	}
	UA_Server_Proxy(int port) {
		_server = UA_Server_new();
		UA_ServerConfig *cc = UA_Server_getConfig(_server);
		UA_ServerConfig_setDefault(cc);
		_config = new UA_ServerConfig_Proxy(cc);
		_mgr = new ServerNodeMgr(_server);
	}
	UA_Server_Proxy(int port, const std::string cert, const std::string pkey) {
		_server = UA_Server_new();
		UA_ServerConfig *cc = UA_Server_getConfig(_server);

		if (cert.length() > 0) {
			UA_ByteString certificate = loadFile(cert.c_str());
			UA_ByteString privateKey  = loadFile(pkey.c_str());

			UA_ByteString *trustList = NULL;
			size_t trustListSize = 0;
			UA_ByteString *issuerList = NULL;
			size_t issuerListSize = 0;
			UA_ByteString *revocationList = NULL;
			size_t revocationListSize = 0;

			UA_ServerConfig_setDefaultWithSecurityPolicies(cc, port, &certificate, &privateKey, trustList, trustListSize, issuerList, issuerListSize, revocationList, revocationListSize);

			UA_ByteString_clear(&certificate);
			UA_ByteString_clear(&privateKey);
		}
		_config = new UA_ServerConfig_Proxy(cc);
		_mgr = new ServerNodeMgr(_server);
	}
	~UA_Server_Proxy() {
		delete _config;
		delete _mgr;
		UA_Server_delete(_server);

		for (auto p : _callbacks) {
			p->func = nullptr;
			delete p;
		}
	}

	struct CallbackData {
		std::function<void (void) > func;
		UA_Server_Proxy* server;
	};
	/*
		CallbackData(UA_Server_Proxy* server, sol::function f, sol::this_state& lua)
			: _server(server), _runner_thread(sol::thread::create(lua))
		{
			_task = new sol::coroutine(_runner_thread.state(), f);
		}
		~CallbackData() {
			delete _task;
		}
		void operator()() {
			std::cout << "ServerCallback Start" << std::endl;
			int ms = (*this->_task)();
			std::cout << "ServerCallback End" << ms << std::endl;
		}
	private:
		UA_Server_Proxy* _server;
		sol::thread _runner_thread;
		sol::coroutine* _task;
	};
	*/
	std::list<CallbackData*> _callbacks;

	static void ServerCallback(UA_Server* server, void* data) {
		CallbackData* p = (CallbackData*)data;
		//(*p)();
		p->func();
	}
	// TODO: for change and remove callback
	UA_StatusCode addCallback(std::function<void(void)> func, int ms) {
	//UA_StatusCode addCallback(sol::function f, int ms, sol::this_state L) {
		if (ms < 5)
			ms = 5;
		//CallbackData* p = new CallbackData(this, f, L);
		CallbackData* p = new CallbackData();
		p->func = func;
		p->server = this;

		_callbacks.push_back(p);
		return UA_Server_addRepeatedCallback(_server, &UA_Server_Proxy::ServerCallback, p, ms, NULL);
	}

	UA_StatusCode run() {
		return UA_Server_run(_server, &_running);
	}

	UA_StatusCode startup() {
		return UA_Server_run_startup(_server);
	}
	UA_UInt16 run_once(bool waitInternal) {
		return UA_Server_run_iterate(_server, waitInternal);
	}
	UA_StatusCode shutdown() {
		return UA_Server_run_shutdown(_server);
	}

	UA_UInt16 addNamespace(const std::string& namespaceUri) {
		return UA_Server_addNamespace(_server, namespaceUri.c_str());
	}
	sol::variadic_results getNamespaceByName(const std::string& namespaceUri, sol::this_state L) {
		size_t index = 0;
		UA_String uri = UA_STRING((char*)namespaceUri.c_str());
		UA_StatusCode re = UA_Server_getNamespaceByName(_server, uri, &index);
		RETURN_RESULT(size_t, index)
	}
	// TODO: UA_Server_getConfig
	//
	ServerNodeMgr* getNodeMgr() {
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

	UA_StatusCode setVariableNode_valueCallback(const UA_NodeId nodeId,
			UA_ValueCallback_Proxy* callback) {
		callback->_proxy = this; // Set the callback
		UA_StatusCode re = UA_Server_setNodeContext(_server, nodeId, callback);
		if (re == UA_STATUSCODE_GOOD) {
			return UA_Server_setVariableNode_valueCallback(_server, nodeId, callback->_callback);
		}
		return re;
	}

	// TODO:
	typedef std::function<UA_StatusCode (UA_Server_Proxy& proxy, const UA_NodeId *sessionId,
                     void *sessionContext, const UA_NodeId *methodId,
                     void *methodContext, const UA_NodeId *objectId,
                     void *objectContext, size_t inputSize,
                     const UA_Variant *input, size_t outputSize,
                     UA_Variant *output) > MethodCallbackFunction;

	struct MethodCallbackData {
		MethodCallbackFunction func;
		UA_Server_Proxy* server;
	};
	std::list<MethodCallbackData*> _method_callbacks;


	static UA_StatusCode MethodCallback(UA_Server *server, const UA_NodeId *sessionId,
                     void *sessionContext, const UA_NodeId *methodId,
                     void *methodContext, const UA_NodeId *objectId,
                     void *objectContext, size_t inputSize,
                     const UA_Variant *input, size_t outputSize,
                     UA_Variant *output) {
			auto p = (MethodCallbackData*)objectContext;
			return p->func(*p->server, sessionId, sessionContext, methodId, methodContext, objectId, objectContext, inputSize, input, outputSize, output);
	}

	UA_StatusCode setMethodCallback(const UA_NodeId methodNodeId, MethodCallbackFunction func) {
		auto p = new MethodCallbackData();
		p->server = this;
		p->func = func;
		_method_callbacks.push_back(p);
		/*
		UA_Server_getNodeContext(_server, methodNodeId, &pfunc);
		if (pfunc) {
		}
		*/
		auto ret = UA_Server_setNodeContext(_server, methodNodeId, p);
		if (UA_STATUSCODE_GOOD == ret)
			return UA_Server_setMethodNode_callback(_server, methodNodeId, &UA_Server_Proxy::MethodCallback);
		else
			return ret;
	}
};

void UA_ValueCallback_Proxy::ReadCallback(UA_Server *server, const UA_NodeId *sessionId,
			   void *sessionContext, const UA_NodeId *nodeId,
			   void *nodeContext, const UA_NumericRange *range,
			   const UA_DataValue *value) {
	UA_ValueCallback_Proxy* p = (UA_ValueCallback_Proxy*)nodeContext;
	// printf("ReadCallback %lld\n", p);
	if (p->_proxy == nullptr || p->_proxy->_server != server) {
		// Code error here
		printf("ReadCallback Error!!!\n");
		return;
	}
	if (p->_read) {
		p->_read(p->_proxy, sessionId, sessionContext, nodeId, range, value);
	}
}

void UA_ValueCallback_Proxy::WriteCallback(UA_Server *server, const UA_NodeId *sessionId,
				void *sessionContext, const UA_NodeId *nodeId,
				void *nodeContext, const UA_NumericRange *range,
				const UA_DataValue *data) {
	UA_ValueCallback_Proxy* p = (UA_ValueCallback_Proxy*)nodeContext;
	// printf("WriteCallback %lld\n", p);
	if (p->_proxy == nullptr || p->_proxy->_server != server) {
		// Code error here
		printf("ReadCallback Error!!!\n");
		return;
	}
	if (p->_write) {
		p->_write(p->_proxy, sessionId, sessionContext, nodeId, range, data);
	}
}

void reg_opcua_server(sol::table& module) {
	module.new_usertype<UA_ValueCallback_Proxy>("ValueCallback",
		sol::constructors<UA_ValueCallback_Proxy(UA_ValueCallback_Proxy::OnReadCallback, UA_ValueCallback_Proxy::OnWriteCallback)>()
	);
	module.new_usertype<UA_ServerConfig_Proxy>("ServerConfig",
		"new", sol::no_constructor, 
		"setProductURI", &UA_ServerConfig_Proxy::setProductURI,
		"setApplicationURI", &UA_ServerConfig_Proxy::setApplicationURI,
		"setApplicationName", &UA_ServerConfig_Proxy::setApplicationName
	);
	module.new_usertype<UA_Server_Proxy>("Server",
		sol::constructors<UA_Server_Proxy(), UA_Server_Proxy(int), UA_Server_Proxy(int, const std::string&, const std::string&)>(),
		"running", sol::readonly(&UA_Server_Proxy::_running),
		"config", &UA_Server_Proxy::_config,
		"addCallback", &UA_Server_Proxy::addCallback,
		"run", &UA_Server_Proxy::run,
		"startup", &UA_Server_Proxy::startup,
		"run_once", &UA_Server_Proxy::run_once,
		"shutdown", &UA_Server_Proxy::shutdown,
		"addNamespace", &UA_Server_Proxy::addNamespace,
		"getNodeMgr", &UA_Server_Proxy::getNodeMgr,
		"getObjectsNode", &UA_Server_Proxy::getObjectsNode,
		"getTypesNode", &UA_Server_Proxy::getTypesNode,
		"getViewsNode", &UA_Server_Proxy::getViewsNode,
		"getServerNode", &UA_Server_Proxy::getServerNode,
		"getRootNode", &UA_Server_Proxy::getRootNode,
		"getNode", sol::overload(
			static_cast<sol::variadic_results (UA_Server_Proxy::*)(const UA_NodeId&, sol::this_state L) >(&UA_Server_Proxy::getNode),
			static_cast<sol::variadic_results (UA_Server_Proxy::*)(int, int, sol::this_state L) >(&UA_Server_Proxy::getNode),
			static_cast<sol::variadic_results (UA_Server_Proxy::*)(int, const char*, sol::this_state L) >(&UA_Server_Proxy::getNode),
			static_cast<sol::variadic_results (UA_Server_Proxy::*)(int, const UA_Guid&, sol::this_state L) >(&UA_Server_Proxy::getNode)
		),
		"deleteNode", sol::overload(
			static_cast<UA_StatusCode (UA_Server_Proxy::*)(const UA_NodeId&, bool) >(&UA_Server_Proxy::deleteNode),
			static_cast<UA_StatusCode (UA_Server_Proxy::*)(const UA_Node&, bool) >(&UA_Server_Proxy::deleteNode)
		),
		"setVariableNode_valueCallback", &UA_Server_Proxy::setVariableNode_valueCallback,
		"setMethodCallback", &UA_Server_Proxy::setMethodCallback
	);

	module.new_usertype<ServerNodeMgr>("ServerNodeMgr",
		sol::constructors<ServerNodeMgr(UA_Server*)>(),
		"addReference", &ServerNodeMgr::addReference,
		"deleteReference", &ServerNodeMgr::deleteReference,
		"deleteNode", &ServerNodeMgr::deleteNode,
		"addObject", &ServerNodeMgr::addObject,
		"addObjectType", &ServerNodeMgr::addObjectType,
		"addVariable", &ServerNodeMgr::addVariable,
		"addDataSourceVariable", &ServerNodeMgr::addDataSourceVariable,
		"addVariableType", &ServerNodeMgr::addVariableType,
		"addReferenceType", &ServerNodeMgr::addReferenceType,
		"addDataType", &ServerNodeMgr::addDataType,
		"addMethod", &ServerNodeMgr::addMethod
	);

}

}
