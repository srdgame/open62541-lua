
## OPCUA Client 客户端

### OPCUA ConnectionConfig - 连接配置对象

#### 构造函数

提供默认的无参数的构造函数

#### 成员变量

* protocolVersion <br> 协议版本
* sendBufferSize <br> 发送缓存大小
* recvBufferSize <br> 接受缓存大小
* maxMessageSize <br> 最大消息长度
* maxChunkCount <br> 最大的数据快数量


### OPCUA ClientConfig 客户端配置对象

#### 构造函数

无默认构造函数

#### 成员变量

N/A

#### 成员函数

* setTimeout(timeout: integer) <br> 设定连接超时时间(ms)
* setSecureChannelLifeTime(life_time: integer) <br> 设定加密通道生命周期长度 (ms)
* setProductURI(uri: string) <br> 设定产品URI
* setApplicationURI(uri: string) <br> 设定应用URI （使用加密认证时需要保证URI跟证书内的URI地址一致)
* setApplicationName(locale: string, name: string) <br> 设定应用名称


### OPCUA Client 客户端对象

#### 构造函数

* new() <br>
初始化一个不使用安全连接的客户端
* new(security_mode | enum, private_cert | string, private_key | string) <br>
初始化安全连接。private_cert和private_key为路径（可以是相对路径)。<br>
支持的模式包含：
    * 1 - None
    * 2 - Sign
    * 3 - SignAndEncrypt


#### 成员变量

* config <br> ClientConfig 对象实例

#### 成员函数

* setStateCallback( callback | function ) <br> 设定状态回调函数，原型为 ``` function(client, state) end```
* getState() <br> 获取当前客户端状态。
* reset() <br> 重置客户端
* connect(endpoint_url | string ) <br> 连接OPCUA服务器
* connect_username(endpoint_url | string, username | string, password | string) <br> 使用用户名密码连接服务器
* disconnect() <br> 断开连接
* manuallyRenewSecureChannel() <br> 手动重新构建安全连接
* getEndpoints(serverUrl | string, endpointDescriptionsSize | size_t \*, endpointDescriptions | EndpointDescription)
<br> 获取连接点信息列表
* findServers(serverUrl | string, serverUrisSize | size_t, serverUris | String, localeIdsSize | size_t, localeIds | String \*, registeredServersSize | size_t \*, registeredServers | ApplicationDescription \*\*)
<br> 差超服务器列表
* getNamespaceIndex(namespaceUri | string) <br> 获取名字空间的序号(ID)
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
* createSubscription(callback | function) <br> 添加订阅，反馈订阅ID。 指定订阅节点数值变化的回调函数，原型为 ``` function(mon_id | integer, value | DataValue, sub_id | integer, context | any) ```
* subscribeNode(subid | UA_UInt32, nodeId | NodeId) <br> 订阅节点，subid是createSubscription返回的ID
* run_iterate(ms | UA_UInt32) <br> 运行指定的时间长度


### OPCUA ClientNodeMgr 客户端节点管理对象

TODO:
