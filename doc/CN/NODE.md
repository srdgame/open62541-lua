## OPCUA Node - 节点对象

### 构造函数

不能直接使用构造函数来构造节点对象。


### 成员变量

* id -- 节点ID (readonly)
* node_class -- 节点类型 (buffered, readonly)
* reference_type -- 引用类型 (buffered, readonly)
* nodeMgr -- NodeMgr 对象实例 (readonly)
* nodeClass -- 节点类型
* browseName -- 浏览名称，QualifiedName
* displayName -- 显示名称, LocalizedText
* description -- 节点描述，LocalizedText
* writeMask -- 写入标志
* userWriteMask -- 用户写入标志
* isAbstract -- 是否是虚节点
* symetric -- boolean
* inverseName -- LocalizedText
* containsNoLoops -- boolean
* eventNotifier -- enum
* value -- 数值， Variant
* dataValue -- 数据数值对象 DataValue
* dataType -- 数据类型 NodeId
* valueRank -- int32
* accessLevel -- 访问级别，AccessLevel enum
* userAccessLevel -- 用户访问级别，UserAccessLevel enum
* minimumSamplingInterval -- 最小的采样时间， double
* historizing -- boolean
* executable -- 是否是可执行的节点，boolean
* userExecutable -- 可否被用户执行，boolean


### 成员函数

* \_\_tostring <br> 字符串化（仅用于日志显示)
* deleteNode(deleteReferences) <br> 删除节点，当deleteReferences为true时，删除这个节点的应用节点
* addFolder(id | NodeId, browse_name | string, attributes | ObjectAttributes) <br> 添加文件夹节点
* addObject(id | NodeId, browse_name | string, attributes | ObjectAttributes) <br> 添加对象节点
* addVariable(id | NodeId, browse_name | string, attributes | ObjectAttributes) <br> 添加变量节点
* addView(id | NodeId, browse_name | string, attributes | ObjectAttributes) <br> 添加View节点
* addMethod(id | NodeId, browse_name | string, attributes | ObjectAttributes) <br> 添加方法节点
* addReference(id | ExpandedNodeId, isForward | bool, uri | string, node_class | NodeClass) <br> 添加引用节点
* deleteReference(id | ExpandedNodeId, isForward | bool, uri | string, node_class | NodeClass) <br> 删除引用节点
* getChildren() <br> 获取子节点
* getChild(name | string) <br> 按名称获取子节点
* getChild(names | table) <br> 按照table中的名称进行深度查找