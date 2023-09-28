
## OPCUA Client classes

### OPCUA ConnectionConfig class

#### Constructors

default new constructor

#### Member Variables

* protocolVersion
* sendBufferSize
* recvBufferSize
* maxMessageSize
* maxChunkCount


### OPCUA ClientConfig class

#### Constructors

default new constructor

#### Member Variables

* timeout
* secureChannelLifeTime 
* localConnectionConfig

### OPCUA Client class

#### Constructors

* new(endpointUrl | string, timeout | integer, secureChannelLifeTime | integer, connectionConfig | ConnectionConfig)

* new(endpointUrl | string, timeout | integer, secureChannelLifeTime | integer, connectionConfig | ConnectionConfig, securityPolicy | string, certificate | string, privateKey | string, remoteCertificate | string)

#### Member Variables

N/A

#### Methods

* getState()
* reset()
* connect()
* connect_username(username | string, password | string)
* disconnect()
* manuallyRenewSecureChannel()
* getEndpoints(serverUrl | string, endpointDescriptionsSize | size_t \*, endpointDescriptions | EndpointDescription)
* findServers(serverUrl | string, serverUrisSize | size_t, serverUris | String, localeIdsSize | size_t, localeIds | String \*, registeredServersSize | size_t \*, registeredServers | ApplicationDescription \*\*)
* getNamespaceIndex(namespaceUri | string)
* getNodeMgr()
* getObjectsNode()
* getTypesNode()
* getViewsNode()
* getServerNode()
* getRootNode()
* getNode(id | NodeId)
* getNode(ns | integer, id | integer)
* getNode(ns | integer, id | string)
* getNode(ns | integer, id | Guid)
* deleteNode(nodeId | NodeId, deleteReferences | boolean)
* deleteNode(node | Node, deleteReferences | boolean)
* createSubscription()
* subscribeNode(subid | UA_UInt32, nodeId | NodeId)
* run_iterate(ms | UA_UInt32)


### OPCUA ClientNodeMgr class

TODO:
