#pragma once
#include <iostream>

#include "open62541.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_interfaces.hpp"

namespace lua_opcua {
extern std::string toString(const UA_NodeId& id);

UA_StatusCode UA_Node_IteratorCallback(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId, void* handle);

class UA_Node;
struct UA_Node_callback {
	const UA_Node* _parent;

	UA_Node_callback(const UA_Node* parent) : _parent(parent) {}
	virtual ~UA_Node_callback(){};
	virtual UA_StatusCode operator()(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId) = 0;
};
struct UA_Node_Iter : public UA_Node_callback {
	std::vector<UA_Node> _childs;

	UA_Node_Iter(const UA_Node* parent) : UA_Node_callback(parent) {}
	UA_StatusCode operator()(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId);
};

struct UA_Node_Finder : public UA_Node_callback {
	UA_String _name;
	int _ns;
	AttributeReader* _reader;
	std::vector<UA_Node> _nodes;
	bool _found;

	UA_Node_Finder(const UA_Node* parent, const std::string& name, AttributeReader* reader);
	~UA_Node_Finder() { UA_String_deleteMembers(&_name); }
	UA_StatusCode operator()(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId);
};

#define MAP_NODE_PROPERTY(PT, PN) \
	PT get##PN() const { \
		PT val; PT##_init(&val); \
		auto reader = _mgr->getAttributeReader(); \
		UA_StatusCode re = reader->read##PN(_id, &val); \
		return val; \
	} \
	UA_StatusCode set##PN(const PT& val) const { \
		auto writer = _mgr->getAttributeWriter(); \
		auto re = writer->write##PN(_id, &val); \
		if (re != UA_STATUSCODE_GOOD) { \
			std::cout << "Write Property to :" << toString(_id) << " err_code: " << UA_StatusCode_name(re) << std::endl; \
		} \
		return re; \
	}

#define SOL_MAP_NODE_PROPERTY(LN, DN) #LN, sol::property(&UA_Node::get##DN, &UA_Node::set##DN)
//#define SOL_MAP_NODE_PROPERTY(LN, DN) "get"#DN, &UA_Node::get##DN, "set"#DN, &UA_Node::set##DN


struct AutoReleaseNodeId {
	UA_NodeId id;
	AutoReleaseNodeId() {
		UA_NodeId_init(&id);
	}
	~AutoReleaseNodeId() {
		UA_NodeId_deleteMembers(&id);
	}
	operator UA_NodeId* () {
		return &id;
	}
};

class UA_Node {
protected:
	friend class UA_Client_Proxy;
	friend class UA_Server_Proxy;
	friend class UA_Node_Iter;
	friend class UA_Node_Finder;
	UA_Node(NodeMgr* mgr, const UA_NodeId id, const UA_NodeId referenceType, UA_NodeClass node_class) : _mgr(mgr), _class(node_class) {
		UA_NodeId_copy(&id, &_id);
		UA_NodeId_copy(&referenceType, &_referenceType);
	}
public:
	NodeMgr* _mgr;
	UA_NodeId _id;
	UA_NodeId _referenceType;
	UA_NodeClass _class;

	virtual ~UA_Node() {
		UA_NodeId_deleteMembers(&_id);
		UA_NodeId_deleteMembers(&_referenceType);
	}
	UA_Node(const UA_Node& obj) {
		_mgr = obj._mgr;
		UA_NodeId_copy(&obj._id, &_id);
		UA_NodeId_copy(&obj._referenceType, &_referenceType);
		_class = obj._class;
	}
	UA_Node& operator=(const UA_Node& that) {
		if(this != &that) {
			_mgr = that._mgr;
			UA_NodeId_copy(&that._id, &_id);
			UA_NodeId_copy(&that._referenceType, &_referenceType);
			_class = that._class;
        }
        return *this;
	}

	operator std::string() const {
		std::stringstream ss;
		ss << "Node(id=" << toString(_id) << ";type=" << toString(_referenceType) << ";class=" << _class << ")";
		return ss.str();
	}

	sol::variadic_results callMethod(const UA_NodeId methodId, sol::as_table_t<std::vector<UA_Variant>> inputs, sol::this_state L) {
		std::vector<UA_Variant> result_vec;
		result_vec.resize(100);
		UA_Variant* p = &result_vec[0];
		size_t out = 100;
		UA_StatusCode re = _mgr->callMethod(_id, methodId, inputs.source.size(), &inputs.source[0], &out, &p);
		result_vec.resize(out);
		/*
		 * RETURN_RESULT(sol::as_returns<UA_Variant>, sol::as_returns(std::move(result_vec)))
		 */
		RETURN_RESULT(std::vector<UA_Variant>, result_vec)
	}

	UA_StatusCode deleteNode(bool deleteReferences) {
		return _mgr->deleteNode(_id, deleteReferences);
	}
	sol::variadic_results addFolder(const UA_NodeId id, const char* browse, const UA_ObjectAttributes attr, sol::this_state L) {
		UA_QualifiedName browse_name = UA_QUALIFIEDNAME_ALLOC(id.namespaceIndex, browse);
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_NodeId type = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
		AutoReleaseNodeId outId;
		UA_StatusCode re = _mgr->addObject(id, _id, referenceTypeId, browse_name, type, attr, outId);
		UA_QualifiedName_deleteMembers(&browse_name);
		UA_NodeId_deleteMembers(&referenceTypeId);
		UA_NodeId_deleteMembers(&type);
		RETURN_RESULT(UA_Node, UA_Node(_mgr, *outId, _referenceType, UA_NODECLASS_OBJECT))
	}
	sol::variadic_results addObject(const UA_NodeId id, const char* browse, const UA_ObjectAttributes attr, sol::this_state L) {
		UA_QualifiedName browse_name = UA_QUALIFIEDNAME_ALLOC(id.namespaceIndex, browse);
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_NodeId type = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
		AutoReleaseNodeId outId;
		UA_StatusCode re = _mgr->addObject(id, _id, referenceTypeId, browse_name, type, attr, outId);
		UA_QualifiedName_deleteMembers(&browse_name);
		UA_NodeId_deleteMembers(&referenceTypeId);
		UA_NodeId_deleteMembers(&type);
		RETURN_RESULT(UA_Node, UA_Node(_mgr, *outId, _referenceType, UA_NODECLASS_OBJECT))
	}
	sol::variadic_results addVariable(const UA_NodeId id, const char* browse, const UA_VariableAttributes attr, sol::this_state L) {
		UA_QualifiedName browse_name = UA_QUALIFIEDNAME_ALLOC(id.namespaceIndex, browse);
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		AutoReleaseNodeId outId;
		UA_StatusCode re = _mgr->addVariable(id, _id, referenceTypeId, browse_name, UA_NODEID_NULL, attr, outId);
		UA_QualifiedName_deleteMembers(&browse_name);
		UA_NodeId_deleteMembers(&referenceTypeId);
		RETURN_RESULT(UA_Node, UA_Node(_mgr, *outId, _referenceType, UA_NODECLASS_VARIABLE))
	}
	sol::variadic_results addView(const UA_NodeId id, const char* browse, const UA_ViewAttributes attr, sol::this_state L) {
		UA_QualifiedName browse_name = UA_QUALIFIEDNAME_ALLOC(id.namespaceIndex, browse);
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		AutoReleaseNodeId outId;
		UA_StatusCode re = _mgr->addView(id, _id, referenceTypeId, browse_name, attr, outId);
		UA_QualifiedName_deleteMembers(&browse_name);
		UA_NodeId_deleteMembers(&referenceTypeId);
		RETURN_RESULT(UA_Node, UA_Node(_mgr, *outId, _referenceType, UA_NODECLASS_VIEW))
	}
	sol::variadic_results addMethod(const UA_NodeId id, const char* browse, const UA_MethodAttributes attr, sol::this_state L) {
		UA_QualifiedName browse_name = UA_QUALIFIEDNAME_ALLOC(id.namespaceIndex, browse);
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		AutoReleaseNodeId outId;
		UA_StatusCode re = _mgr->addMethod(id, _id, referenceTypeId, browse_name, attr, NULL, 0, NULL, 0, NULL, NULL, outId);
		UA_QualifiedName_deleteMembers(&browse_name);
		UA_NodeId_deleteMembers(&referenceTypeId);
		RETURN_RESULT(UA_Node, UA_Node(_mgr, *outId, _referenceType, UA_NODECLASS_METHOD))
	}
	sol::variadic_results addReference(const UA_ExpandedNodeId id, bool isForward, const char* uri, UA_NodeClass node_class, sol::this_state L) {
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_String targetServerUri = UA_STRING_ALLOC(uri);
		UA_StatusCode re = _mgr->addReference(_id, referenceTypeId, id, isForward, targetServerUri, node_class);
		UA_NodeId_deleteMembers(&referenceTypeId);
		UA_String_deleteMembers(&targetServerUri);
		RETURN_RESULT(bool, true)
	}
	sol::variadic_results deleteReference(const UA_ExpandedNodeId id, bool isForward, UA_NodeClass node_class, sol::this_state L) {
		UA_NodeId referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
		UA_StatusCode re = _mgr->deleteReference(_id, referenceTypeId, id, isForward, node_class);
		UA_NodeId_deleteMembers(&referenceTypeId);
		RETURN_RESULT(bool, true)
	}
	sol::as_table_t< std::vector<UA_Node> > getChildren() const {
		//std::cout << "getChildren\t" << toString(_id) << "\t" << toString(_referenceType) << std::endl;
		UA_Node_Iter iter(this);
		UA_StatusCode re = _mgr->forEachChildNodeCall(_id, UA_Node_IteratorCallback, &iter);
		UA_NodeClass outNodeClass;
		auto reader = _mgr->getAttributeReader();
		for (auto & node : iter._childs) {
			auto ret = reader->readNodeClass(node._id, &outNodeClass);
			if (UA_STATUSCODE_GOOD == ret) {
				node._class = outNodeClass;
				//std::cout << "!!!! Read NodeClass\t" << outNodeClass << "\t" << node._class << std::endl;
			} else {
				//std::cout << "Failed to read NodeClass\t" << ret << std::endl;
			}
		}
		return sol::as_table_t< std::vector<UA_Node> >( iter._childs );
	}
	bool _getChild(const std::string& name, std::vector<UA_Node>& result) {
		UA_Node_Finder op(this, name, _mgr->getAttributeReader());
		UA_StatusCode re = _mgr->forEachChildNodeCall(_id, UA_Node_IteratorCallback, &op);
		if (op._found) {
			result.insert(result.end(), op._nodes.begin(), op._nodes.end());
			UA_NodeClass outNodeClass;
			auto reader = _mgr->getAttributeReader();
			for (auto & node: result) {
				auto ret = reader->readNodeClass(node._id, &outNodeClass);
				if (UA_STATUSCODE_GOOD == ret) {
					node._class = outNodeClass;
					//std::cout << "Read NodeClass\t" << outNodeClass << "\t" << node._class << std::endl;
				} else {
					//std::cout << "Failed to read NodeClass\t" << ret << std::endl;
				}
			}
		}
		return op._found;
	}
	sol::variadic_results getChild(const std::string& name, sol::this_state L) {
		//std::cout << "getChild in name" << std::endl;
		sol::variadic_results result;
		std::vector<UA_Node> nodes;
		bool found = _getChild(name, nodes);
		if (found) { 
			for(auto & node : nodes) {
				result.push_back({ L, sol::in_place_type<UA_Node>, node});
			}
		} else {
			result.push_back({ L, sol::in_place_type<sol::lua_nil_t>, sol::lua_nil_t()});
			result.push_back({ L, sol::in_place_type<std::string>, std::string("Not found!")});
		}
		return result;
	}
	void _getChild(std::vector<UA_Node>& nodes, const std::string& name, std::vector<UA_Node>& results) {
		for(auto parent : nodes) {
			parent._getChild(name, results);
		}
	}
	sol::variadic_results getChild(const sol::as_table_t<std::vector<std::string> > names, sol::this_state L) {
		//std::cout << "getChild in vector" << std::endl;
		sol::variadic_results result;
		auto ptr = names.source.begin();

		std::vector<UA_Node> results;
		results.push_back(*this);
		for(; ptr != names.source.end(); ++ptr) {
			std::string name = *ptr;
			std::vector<UA_Node> founds; 
			_getChild(results, name, founds);
			if (founds.empty())
			{
				result.push_back({ L, sol::in_place_type<sol::lua_nil_t>, sol::lua_nil_t()});
				result.push_back({ L, sol::in_place_type<std::string>, name + " Not found!"});
				return result;
			}
			founds.swap(results);
		}
		for (auto node : results)
			result.push_back({ L, sol::in_place_type<UA_Node>, node});
		return result;
	}

	MAP_NODE_PROPERTY(UA_NodeClass, NodeClass)
	MAP_NODE_PROPERTY(UA_QualifiedName, BrowseName)
	MAP_NODE_PROPERTY(UA_LocalizedText, DisplayName)
	MAP_NODE_PROPERTY(UA_LocalizedText, Description)
	MAP_NODE_PROPERTY(UA_UInt32, WriteMask)
	MAP_NODE_PROPERTY(UA_UInt32, UserWriteMask)
	MAP_NODE_PROPERTY(UA_Boolean, IsAbstract)
	MAP_NODE_PROPERTY(UA_Boolean, Symmetric)
	MAP_NODE_PROPERTY(UA_LocalizedText, InverseName)
	MAP_NODE_PROPERTY(UA_Boolean, ContainsNoLoops)
	MAP_NODE_PROPERTY(UA_Byte, EventNotifier)
	MAP_NODE_PROPERTY(UA_Variant, Value)
	MAP_NODE_PROPERTY(UA_DataValue, DataValue)
	MAP_NODE_PROPERTY(UA_NodeId, DataType)
	MAP_NODE_PROPERTY(UA_Int32, ValueRank)
	// ArrayDimensions TODO:
	MAP_NODE_PROPERTY(UA_Byte, AccessLevel)
	MAP_NODE_PROPERTY(UA_Byte, UserAccessLevel)
	MAP_NODE_PROPERTY(UA_Double, MinimumSamplingInterval)
	MAP_NODE_PROPERTY(UA_Boolean, Historizing)
	MAP_NODE_PROPERTY(UA_Boolean, Executable)
	MAP_NODE_PROPERTY(UA_Boolean, UserExecutable)
};

}
