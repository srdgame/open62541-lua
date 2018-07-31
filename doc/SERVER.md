
## OPCUA Server classes

### OPCUA ServerConfig class

#### Constructors

No default new constructor

#### Member Variables

N/A

#### Methods

* setProductURI(uri | string)

* setServerURI(uri | string)

* setServername(locale | string, name | string)

### OPCUA Server class

#### Constructors

* new()

* new(port | int)

* new(port | int, cert | string)

#### Member Variables

* running
boolean -- Is server running flag.

* config
ServerConfig -- Server configuration.

#### Methods
* addCallback(func | function, ms | integer)
Add server repeated callback function.

* run()
Server run loop.

* startup()
Server run startup.

* run_once()
Server run once.

* shutdown()
Server run shutdown.

* addNamespace(namespaceUri | string)
Add namespace.

* getNamespaceByName(namespaceUri | string)
Get namespace by name.

* getNodeMgr()
Get sever node mananger.

* getObjectsNode()
* getTypesNode()
* getViewsNode()
* getServerNode()
* getRootNode()
* getNode(id | NodeId)
* getNode(ns | int, id | int)
* getNode(ns | int, id | string)
* getNode(ns | int, id | guid)
* deleteNode(id | NodeId, deleteReference | boolean)
* deleteNode(id | Node, deleteReference | boolean)
* setMethodCallback(methodNodeId | NodeId, func | MethodCallbackFunction)

### OPCUA ClientNodeMgr class

TODO:
