# OpcUa(open61850) Lua binding

## Functions

* [setLogger](#setlogger)
* [getStatusCodeName](#getstatuscodename)

#### setLogger
> Set logger callback method

_Usage:_
``` lua
local opcua = require 'opcua'
opcua.setLogger(function(level, category, msg)
	print(level, category, msg)
end)
```

#### getStatusCodeName
> Get cosponding string name of UA_StatusCode enums

_Usage:_
``` lua
local r = client:connect()
print(opcua.getStatusCodeName(r))
```
