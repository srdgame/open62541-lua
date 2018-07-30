## OPCUA Node class

### Constructor
No public constructor


### Member Vars

* id -- NodeId class (readonly)
* node_class -- NodeClass class (buffered, readonly)
* reference_type -- ReferenceType (buffered, readonly)
* nodeMgr -- NodeMgr class instance (readonly)
* nodeClass -- NodeClass getter/setter
* browseName -- QualifiedName getter/setter
* displayName -- LocalizedText getter/setter
* description -- LocalizedText getter/setter
* writeMask -- Write Mask enum getter/setter
* userWriteMask -- User Write Mask enum getter/setter
* isAbstract -- boolean getter/setter
* symetric -- boolean getter/setter
* inverseName -- LocalizedText getter/setter
* containsNoLoops -- boolean getter/setter
* eventNotifier -- enum getter/setter
* value -- Variant getter/setter
* dataValue -- DataValue getter/setter
* dataType -- NodeId getter/setter
* valueRank -- int32 getter/setter
* accessLevel -- AccessLevel enum getter/setter
* userAccessLevel -- UserAccessLevel enum getter/setter
* minimumSamplingInterval -- double getter/setter
* historizing -- boolean getter/setter
* executable -- boolean getter/setter
* userExecutable -- boolean getter/setter


### Methods

* \_\_tostring

* deleteNode(deleteReferences)
Delete this node. Delete reference if deleteReferences is true

* addFolder(id | NodeId, browse_name | string, attributes | ObjectAttributes)
Add one folder child

* addObject(id | NodeId, browse_name | string, attributes | ObjectAttributes)
Add one object child

* addVariable(id | NodeId, browse_name | string, attributes | ObjectAttributes)
Add one variable child

* addView(id | NodeId, browse_name | string, attributes | ObjectAttributes)
Add one view child

* addMethod(id | NodeId, browse_name | string, attributes | ObjectAttributes)
Add one method child

* addReference(id | ExpandedNodeId, isForward | bool, uri | string, node_class | NodeClass)
Add one reference child

* deleteReference(id | ExpandedNodeId, isForward | bool, uri | string, node_class | NodeClass)
Delete once reference child

* getChildren()
Get children of this node

* getChild(name | string)
Get child node by browse name

* getChild(names | table)
Get child node by browse name list
