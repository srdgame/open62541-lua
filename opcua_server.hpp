#pragma once
#include <iostream>
#include <list>

#include "open62541.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_interfaces.hpp"
#include "opcua_node.hpp"

namespace lua_opcua {

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
		return UA_Server_addMethodNode(_server, requestedNewNodeId, parentNodeId, referenceTypeId, browseName, attr, method, inputArgumentsSize, inputArguments, outputArgumentsSize, outputArguments, nodeContext, outNewNodeId);
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
	}
public:
	void setProductURI(const std::string& uri) {
		/*
		UA_String_deleteMembers(&_config->buildInfo.productUri);
		_config->buildInfo.productUri = UA_STRING_ALLOC((char*)uri.c_str())
		*/
		UA_String_deleteMembers(&_config->applicationDescription.productUri);
		_config->applicationDescription.productUri = UA_STRING_ALLOC((char*)uri.c_str());
	}
	void setServerURI(const std::string& uri) {
		UA_String_deleteMembers(&_config->applicationDescription.applicationUri);
		_config->applicationDescription.applicationUri = UA_STRING_ALLOC((char*)uri.c_str());
	}
	void setServerName(const std::string& locale, const std::string& name) {
		UA_LocalizedText_deleteMembers(&_config->applicationDescription.applicationName);
		_config->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC(locale.c_str(), name.c_str());
	}
};

void UA_LUA_Logger(UA_LogLevel level, UA_LogCategory category, const char *msg, va_list args);
class UA_Server_Proxy {
protected:
	UA_Server_Proxy(UA_Server_Proxy& prox);
	UA_Server* _server;
	ServerNodeMgr* _mgr;
	UA_ServerConfig *_config;

public:
	volatile bool _running = true;
	UA_ServerConfig_Proxy *_config_proxy;

public:
	UA_Server_Proxy() {
		_config = UA_ServerConfig_new_default();
		_config->logger = UA_LUA_Logger;
		_config_proxy = new UA_ServerConfig_Proxy(_config);

		_server = UA_Server_new(_config);
		_mgr = new ServerNodeMgr(_server);
	}
	UA_Server_Proxy(int port) {
		_config = UA_ServerConfig_new_minimal(port, NULL);
		_config->logger = UA_LUA_Logger;
		_config_proxy = new UA_ServerConfig_Proxy(_config);

		_server = UA_Server_new(_config);
		_mgr = new ServerNodeMgr(_server);
	}
	UA_Server_Proxy(int port, const std::string cert) {
		auto cert_s = UA_BYTESTRING_ALLOC(cert.c_str());
		_config = UA_ServerConfig_new_minimal(port, &cert_s);
		UA_ByteString_deleteMembers(&cert_s);
		_config->logger = UA_LUA_Logger;
		_config_proxy = new UA_ServerConfig_Proxy(_config);

		_server = UA_Server_new(_config);
		_mgr = new ServerNodeMgr(_server);
	}
	~UA_Server_Proxy() {
		delete _config_proxy;
		delete _mgr;
		UA_Server_delete(_server);
		UA_ServerConfig_delete(_config);

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
		UA_Server_setNodeContext(_server, methodNodeId, p);
		UA_Server_setMethodNode_callback(_server, methodNodeId, &UA_Server_Proxy::MethodCallback);
	}
};

void reg_opcua_server(sol::table& module) {
	module.new_usertype<UA_ServerConfig_Proxy>("ServerConfig",
		"new", sol::no_constructor, 
		"setProductURI", &UA_ServerConfig_Proxy::setProductURI,
		"setServerURI", &UA_ServerConfig_Proxy::setServerURI,
		"setServerName", &UA_ServerConfig_Proxy::setServerName
	);
	module.new_usertype<UA_Server_Proxy>("Server",
		sol::constructors<UA_Server_Proxy(), UA_Server_Proxy(int), UA_Server_Proxy(int, const std::string&)>(),
		"running", sol::readonly(&UA_Server_Proxy::_running),
		"config", &UA_Server_Proxy::_config_proxy,
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
		"addVariableType", &ServerNodeMgr::addVariableType,
		"addReferenceType", &ServerNodeMgr::addReferenceType,
		"addDataType", &ServerNodeMgr::addDataType
		//"addMethod", &ServerNodeMgr::addMethod
	);

}

}
