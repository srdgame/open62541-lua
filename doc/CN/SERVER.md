
## OPCUA Server 服务器对象

### OPCUA ServerConfig - 服务器配置对象

#### 构造函数

无构造函数，无法直接构造此对象

#### 成员变量

N/A

#### 成员函数

* setProductURI(uri | string) <br> 设定产品URI
* setServerURI(uri | string) <br> 设定服务器URI
* setServername(locale | string, name | string) <br> 设定服务器名称

### OPCUA Server 服务器对象

#### 构造函数

* new() <br> 默认构造函数
* new(port | int) <br> 指定服务器端口
* new(port | int, cert | string， private_key | string) <br> 指定服务器端口，以及安全证书(cert), 服务器私钥(private_key)。支持相对路径

#### 成员变量

* running<br>
服务器运行状态，boolean变量
* config <br>
服务器配置信息， ServerConfig类型对象

#### 成员函数
* addCallback(func | function, ms | integer) <br>
添加服务器周期性回调
* run() <br>
服务器运行入口
* startup() <br>
服务器初始化
* run_once() <br>
服务器运行一次
* shutdown() <br>
关闭服务器
* addNamespace(namespaceUri | string) <br>
添加命名空间，返回明明空间的ID
* getNamespaceByName(namespaceUri | string) <br>
获取制定URI的命名空间
* getNodeMgr() <br>
获取节点管理器对象
* getNodeMgr() <br> 获取节点管理器
* getObjectsNode() <br> 获取Objects根节点
* getTypesNode() <br> 获取Types根节点
* getViewsNode() <br> 获取Views根节点
* getServerNode() <br> 获取Server根节点
* getRootNode() <br> 获取系统根节点
* getNode(id | NodeId) <br> 获取节点对象
* getNode(ns | integer, id | integer) <br> 获取节点对象（指定名字空间和整数的ID)
* getNode(ns | integer, id | string) <br> 获取节点对象（指定名字空间和字符串的ID)
* getNode(ns | integer, id | Guid) <br> 获取节点对象（指定名字空间和UUID的ID)
* deleteNode(nodeId | NodeId, deleteReferences | boolean) <br> 删除节点，deleteReference为true时，删除节点的所有引用节点
* deleteNode(node | Node, deleteReferences | boolean) <br> 删除节点，deleteReference为true时，删除节点的所有引用节点
* setMethodCallback(methodNodeId | NodeId, func | MethodCallbackFunction) <br> 设定方法 ID的回调函数
* setVariableNode_valueCallback(id | NodeId, callback | ValueCallback) <br> 设定变量节点数据变化回调函数对象

### OPCUA ServerNodeMgr 节点管理器对象

TODO:

### ValueCallback 对象

* new(func_on_read, func_on_write) <br>
指定回调对象对应的读取、写入回调函数:

#### 回调函数原型

``` lua
on_read = function(server | Server, sessionId | NodeId, sessionContext | c_data, nodeId | NodeId, includeSourceTimestamp | boolean, range | Numericrage, value | DataValue)
end
on_write = fuinction(server | Server, sessionId | NodeId, sessionContext | c_data, nodeId | NodeId, range | Numericrage, value | DataValue)
```

可在on_read函数中直接改变 value (DataValue)中的数据，客户端将读取到这个被改变后的数据。


#### 使用

* on_read  <br>  This will be called when client try to read value of variable. You cloud change the dataValue during on read callback
* on_write  <br> This will be called when client write value to variable. The dataValue has the value written from client.
