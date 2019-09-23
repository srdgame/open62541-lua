# OpcUa(open61850) Lua binding 模块

## 全局函数

* [setLogger](#setlogger)
* [getStatusCodeName](#getstatuscodename)

#### setLogger

设定日志输出回调函数

_Usage:_
``` lua
local opcua = require 'opcua'
opcua.setLogger(function(level, category, msg)
	print(level, category, msg)
end)
```

#### getStatusCodeName

获取状态码对应的字符串化名称

_Usage:_
``` lua
local r = client:connect()
print(opcua.getStatusCodeName(r))
```
