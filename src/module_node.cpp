#include "module_node.hpp"

namespace lua_opcua {

UA_StatusCode UA_Node_Iter::operator()(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId) {
	if (isInverse)
		return UA_STATUSCODE_GOOD;

	//std::cout << "UA_Node_Iter\t" << toString(childId) << "\t" << toString(referenceTypeId) << std::endl;
	UA_Node node(_parent->_mgr, childId, referenceTypeId, UA_NODECLASS_UNSPECIFIED);
	_childs.push_back(node);
	return UA_STATUSCODE_GOOD;
}

UA_Node_Finder::UA_Node_Finder(const UA_Node* parent, const std::string& name, AttributeReader* reader)
	: UA_Node_callback(parent), _reader(reader), _found(false)
{
	auto index = name.find(":");
	if (index != name.npos) {
		std::stringstream ss;
		ss << name.substr(0, index);
		ss >> _ns;
		//std::cout << "autoQualifiedName ns:" << _ns << " name:" << name.substr(index+1) << std::endl;
		_name = UA_STRING_ALLOC(name.substr(index + 1).c_str());
	} else {
		_ns = _parent->_id.namespaceIndex;
		_name = UA_STRING_ALLOC(name.c_str());
	}
}

UA_StatusCode UA_Node_Finder::operator()(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId) {
	if (childId.namespaceIndex != _ns)
		return UA_STATUSCODE_GOOD;

	UA_Node node(_parent->_mgr, childId, referenceTypeId, UA_NODECLASS_OBJECT);
	UA_QualifiedName browse_name; UA_QualifiedName_init(&browse_name);
	UA_StatusCode re = _reader->readBrowseName(childId, &browse_name);
	//std::cout << "UA_Node_Finder ns:" << browse_name.namespaceIndex << "," << childId.namespaceIndex << " name:" << (char*)browse_name.name.data << std::endl;
	if (re == UA_STATUSCODE_GOOD && UA_String_equal(&browse_name.name, &_name)) {
		_nodes.push_back(node);
		_found = true;
	}
	return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Node_IteratorCallback(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId, void* handle) {
	UA_Node_callback* cb = (UA_Node_callback*)handle;
	return (*cb)(childId, isInverse, referenceTypeId);
}


void reg_opcua_node(sol::table& module) {
	module.new_usertype<UA_Node>("Node",
		"new", sol::no_constructor,
		"id", sol::readonly(&UA_Node::_id),
		"__tostring", [](const UA_Node& node) { return (std::string)node; },
		"node_class", sol::readonly(&UA_Node::_class),
		"reference_type", sol::readonly(&UA_Node::_referenceType),
		"nodeMgr", sol::readonly(&UA_Node::_mgr),
		"deleteNode", &UA_Node::deleteNode,
		"addFolder", &UA_Node::addFolder,
		"addObject", &UA_Node::addObject,
		"addVariable", &UA_Node::addVariable,
		"addView", &UA_Node::addView,
		"addReference", &UA_Node::addReference,
		"deleteReference", &UA_Node::deleteReference,
		"getChildren", &UA_Node::getChildren,
		"getChild", sol::overload(
			static_cast<sol::variadic_results (UA_Node::*)(const std::string& name, sol::this_state L) >(&UA_Node::getChild),
			static_cast<sol::variadic_results (UA_Node::*)(const sol::as_table_t<std::vector<std::string> > names, sol::this_state L) >(&UA_Node::getChild)
		),
		SOL_MAP_NODE_PROPERTY(nodeClass, NodeClass),
		SOL_MAP_NODE_PROPERTY(browseName, BrowseName),
		SOL_MAP_NODE_PROPERTY(displayName, DisplayName),
		SOL_MAP_NODE_PROPERTY(description, Description),
		SOL_MAP_NODE_PROPERTY(writeMask, WriteMask),
		SOL_MAP_NODE_PROPERTY(userWriteMask, UserWriteMask),
		SOL_MAP_NODE_PROPERTY(isAbstract, IsAbstract),
		SOL_MAP_NODE_PROPERTY(symetric, Symmetric),
		SOL_MAP_NODE_PROPERTY(inverseName, InverseName),
		SOL_MAP_NODE_PROPERTY(containsNoLoops, ContainsNoLoops),
		SOL_MAP_NODE_PROPERTY(eventNotifier, EventNotifier),
		SOL_MAP_NODE_PROPERTY(value, Value),
		SOL_MAP_NODE_PROPERTY(dataValue, DataValue),
		SOL_MAP_NODE_PROPERTY(dataType, DataType),
		SOL_MAP_NODE_PROPERTY(valueRank, ValueRank),
		// ArrayDimensions TODO:
		SOL_MAP_NODE_PROPERTY(accessLevel, AccessLevel),
		SOL_MAP_NODE_PROPERTY(userAccessLevel, UserAccessLevel),
		SOL_MAP_NODE_PROPERTY(minimumSamplingInterval, MinimumSamplingInterval),
		SOL_MAP_NODE_PROPERTY(historizing, Historizing),
		SOL_MAP_NODE_PROPERTY(executable, Executable),
		SOL_MAP_NODE_PROPERTY(userExecutable, UserExecutable)
	);
}

}
