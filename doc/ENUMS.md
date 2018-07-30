## OPCUA Enums

### UA_ClientState
Wrapped without prefix

* DICONNECTED ( UA_CLIENTSTATE_DISCONNECTED )
* CONNECTED ( UA_CLIENTSTATE_CONNECTED )
* SECURECHANNEL ( UA_CLIENTSTATE_SECURECHANNEL )
* SESSION ( UA_CLIENTSTATE_SESSION )
* SESSION_RENEWED ( UA_CLIENTSTATE_SESSION_RENEWED )

Usage:
``` lua
	local state = opcua.UA_ClientState.DISCONNECTED
```

### VariantType
Wrapped without prefix

e.g.
* UA_TYPES_BOOLEAN => opcua.VariantType.BOOLEAN
* UA_TYPES_NODEID => opcua.VariantType.NODEID


### AccessLevel

* UA_ACCESSLEVELMASK_READ => opcua.AccessLevel.READ
* UA_ACCESSLEVELMASK_WRITE => opcua.AccessLevel.READ

except:
opcua.AccessLevel.RW = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE
opcua.AccessLevel.ALL = 0xF


### WriteMask

* UA_WRITEMASK_ACCESSLEVEL => opcua.WriteMask.ACCESSLEVEL
* UA_WRITEMASK_DATATYPE => opcua.WriteMask.DATATYPE


### LogLevel

* UA_LogLevel::UA_LOGLEVEL_TRACE => opcua.LogLevel.TRACE
* UA_LogLevel::UA_LOGLEVEL_DEBUG => opcua.LogLevel.DEBUG


### LogCategory

* UA_LogCategory::UA_LOGCATEGORY_NETWORK => opcua.LogCategory.NETWORK
* UA_LogCategory::UA_LOGCATEGORY_SECURECHANNEL => opcua.LogCategory.SECURECHANNEL

