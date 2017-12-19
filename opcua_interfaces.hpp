#pragma once

#include "open62541.h"

namespace lua_opcua {

class AttributeReader {
public:
	virtual UA_StatusCode readNodeId(const UA_NodeId nodeId, UA_NodeId *outNodeId) = 0;
	virtual UA_StatusCode readNodeClass(const UA_NodeId nodeId, UA_NodeClass *outNodeClass) = 0;
	virtual UA_StatusCode readBrowseName(const UA_NodeId nodeId, UA_QualifiedName *outBrowseName) = 0;
	virtual UA_StatusCode readDisplayName(const UA_NodeId nodeId, UA_LocalizedText *outDisplayName) = 0;
	virtual UA_StatusCode readDescription(const UA_NodeId nodeId, UA_LocalizedText *outDescription) = 0;
	virtual UA_StatusCode readWriteMask(const UA_NodeId nodeId, UA_UInt32 *outWriteMask) = 0;
	virtual UA_StatusCode readUserWriteMask(const UA_NodeId nodeId, UA_UInt32 *outUserWriteMask) = 0;
	virtual UA_StatusCode readIsAbstract(const UA_NodeId nodeId, UA_Boolean *outIsAbstract) = 0;
	virtual UA_StatusCode readSymmetric(const UA_NodeId nodeId, UA_Boolean *outSymmetric) = 0;
	virtual UA_StatusCode readInverseName(const UA_NodeId nodeId, UA_LocalizedText *outInverseName) = 0;
	virtual UA_StatusCode readContainsNoLoops(const UA_NodeId nodeId, UA_Boolean *outContainsNoLoops) = 0;
	virtual UA_StatusCode readEventNotifier(const UA_NodeId nodeId, UA_Byte *outEventNotifier) = 0;
	virtual UA_StatusCode readValue(const UA_NodeId nodeId, UA_Variant *outValue) = 0;
	virtual UA_StatusCode readDataValue(const UA_NodeId nodeId, UA_DataValue *outDataValue) = 0;
	virtual UA_StatusCode readDataType(const UA_NodeId nodeId, UA_NodeId *outDataType) = 0;
	virtual UA_StatusCode readValueRank(const UA_NodeId nodeId, UA_Int32 *outValueRank) = 0;
	virtual UA_StatusCode readArrayDimensions(const UA_NodeId nodeId, size_t *outArrayDimensionsSize, UA_UInt32 **outArrayDimensions) = 0;
	virtual UA_StatusCode readAccessLevel(const UA_NodeId nodeId, UA_Byte *outAccessLevel) = 0;
	virtual UA_StatusCode readUserAccessLevel(const UA_NodeId nodeId, UA_Byte *outUserAccessLevel) = 0;
	virtual UA_StatusCode readMinimumSamplingInterval(const UA_NodeId nodeId, UA_Double *outMinSamplingInterval) = 0;
	virtual UA_StatusCode readHistorizing(const UA_NodeId nodeId, UA_Boolean *outHistorizing) = 0;
	virtual UA_StatusCode readExecutable(const UA_NodeId nodeId, UA_Boolean *outExecutable) = 0;
	virtual UA_StatusCode readUserExecutable(const UA_NodeId nodeId, UA_Boolean *outUserExecutable) = 0;
};

class AttributeWriter {
public:
	virtual UA_StatusCode writeNodeId(const UA_NodeId nodeId, const UA_NodeId *newNodeId) = 0;
	virtual UA_StatusCode writeNodeClass(const UA_NodeId nodeId, const UA_NodeClass *newNodeClass) = 0;
	virtual UA_StatusCode writeBrowseName(const UA_NodeId nodeId, const UA_QualifiedName *newBrowseName) = 0;
	virtual UA_StatusCode writeDisplayName(const UA_NodeId nodeId, const UA_LocalizedText *newDisplayName) = 0;
	virtual UA_StatusCode writeDescription(const UA_NodeId nodeId, const UA_LocalizedText *newDescription) = 0;
	virtual UA_StatusCode writeWriteMask(const UA_NodeId nodeId, const UA_UInt32 *newWriteMask) = 0;
	virtual UA_StatusCode writeUserWriteMask(const UA_NodeId nodeId, const UA_UInt32 *newUserWriteMask) = 0;
	virtual UA_StatusCode writeIsAbstract(const UA_NodeId nodeId, const UA_Boolean *newIsAbstract) = 0;
	virtual UA_StatusCode writeSymmetric(const UA_NodeId nodeId, const UA_Boolean *newSymmetric) = 0;
	virtual UA_StatusCode writeInverseName(const UA_NodeId nodeId, const UA_LocalizedText *newInverseName)  = 0;
	virtual UA_StatusCode writeContainsNoLoops(const UA_NodeId nodeId, const UA_Boolean *newContainsNoLoops) = 0;
	virtual UA_StatusCode writeEventNotifier(const UA_NodeId nodeId, const UA_Byte *newEventNotifier) = 0;
	virtual UA_StatusCode writeValue(const UA_NodeId nodeId, const UA_Variant *newValue) = 0;
	virtual UA_StatusCode writeDataValue(const UA_NodeId nodeId, const UA_DataValue *newDataValue) = 0;
	virtual UA_StatusCode writeDataType(const UA_NodeId nodeId, const UA_NodeId *newDataType) = 0;
	virtual UA_StatusCode writeValueRank(const UA_NodeId nodeId, const UA_Int32 *newValueRank) = 0;
	virtual UA_StatusCode writeArrayDimensions(const UA_NodeId nodeId, size_t newArrayDimensionsSize, const UA_UInt32 *newArrayDimensions) = 0;
	virtual UA_StatusCode writeAccessLevel(const UA_NodeId nodeId, const UA_Byte *newAccessLevel) = 0;
	virtual UA_StatusCode writeUserAccessLevel(const UA_NodeId nodeId, const UA_Byte *newUserAccessLevel) = 0;
	virtual UA_StatusCode writeMinimumSamplingInterval(const UA_NodeId nodeId, const UA_Double *newMinInterval) = 0;
	virtual UA_StatusCode writeHistorizing(const UA_NodeId nodeId, const UA_Boolean *newHistorizing) = 0;
	virtual UA_StatusCode writeExecutable(const UA_NodeId nodeId, const UA_Boolean *newExecutable) = 0;
	virtual UA_StatusCode writeUserExecutable(const UA_NodeId nodeId, const UA_Boolean *newUserExecutable) = 0;
};

class NodeMgr {
public:
	virtual AttributeReader* getAttributeReader() = 0;
	virtual AttributeWriter* getAttributeWriter() = 0;
	virtual UA_StatusCode addReference(const UA_NodeId sourceNodeId,
			const UA_NodeId referenceTypeId,
			const UA_ExpandedNodeId targetNodeId,
			UA_Boolean isForward,
			const UA_String targetServerUri,
			UA_NodeClass targetNodeClass) = 0;
	virtual UA_StatusCode deleteReference(const UA_NodeId sourceNodeId,
			const UA_NodeId referenceTypeId,
			const UA_ExpandedNodeId targetNodeId,
			UA_Boolean isForward,
			UA_NodeClass targetNodeClass) = 0;
	virtual UA_StatusCode deleteNode(const UA_NodeId nodeId, bool deleteTargetReferences) = 0;
	virtual UA_StatusCode addVariable(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addVariableType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_VariableTypeAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addObject(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_NodeId typeDefinition,
			const UA_ObjectAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addObjectType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ObjectTypeAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addView(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ViewAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addReferenceType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_ReferenceTypeAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addDataType(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_DataTypeAttributes attr,
			UA_NodeId *outNewNodeId) = 0;
	virtual UA_StatusCode addMethod(const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId,
			const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName,
			const UA_MethodAttributes attr,
			UA_MethodCallback method,
			size_t inputArgumentsSize, const UA_Argument* inputArguments,
			size_t outputArgumentsSize, const UA_Argument* outputArguments,
			void *nodeContext,
			UA_NodeId *outNewNodeId)  = 0;
	virtual UA_StatusCode callMethod(const UA_NodeId objectId,
			const UA_NodeId methodId,
			size_t inputSize,
			const UA_Variant *input,
            size_t *outputSize,
			UA_Variant **output) = 0;
	virtual UA_StatusCode forEachChildNodeCall(UA_NodeId parentNodeId, 
			UA_NodeIteratorCallback callback,
			void *handle) = 0;
};

#define RETURN_RESULT(XT, X) \
	sol::variadic_results result; \
	if (re == UA_STATUSCODE_GOOD) { \
		result.push_back({ L, sol::in_place_type<XT>, X}); \
	} else { \
		result.push_back({ L, sol::in_place_type<sol::lua_nil_t>, sol::lua_nil_t()}); \
		result.push_back({ L, sol::in_place_type<std::string>, std::string(UA_StatusCode_name(re))}); \
	} \
	return result; \


#define MAP_PROPERTY(CLASS, DT, DN) \
	#DN, sol::property( \
		[](CLASS& obj) { \
			DT val; DT##_init(&val); \
			DT##_copy(&obj.DN, &val); return val; \
		}, \
		[](CLASS& obj, const DT& val) { \
			DT##_deleteMembers(&obj.DN); \
			DT##_copy(&val, &obj.DN); \
		} \
	)

#define MAP_PROPERTY_HAS(CLASS, DT, DN, hasDN) \
	#DN, sol::property( \
		[](CLASS& obj) { \
			DT val; DT##_init(&val); \
			DT##_copy(&obj.DN, &val); return val; \
		}, \
		[](CLASS& obj, const DT& val) { \
			DT##_deleteMembers(&obj.DN); \
			DT##_copy(&val, &obj.DN); \
			obj.hasDN = true; \
		} \
	)


}
