# OPCUA 基础数据类型

## 目录
* [DateTime](#datetime)
* [DateTimeStruct](#datetimestuct)
* [Variant](#variant)
* [DataValue](#datavalue)
* [NodeId](#nodid)
* [ExpandedNodeId](#expandednodeid)
* [QualifiedName](#qualifiedname)
* [LocalizedText](#localizedtext)
* [ObjectAttributes](#objectattributes)
* [VariantAttributes](#variantattributes)
* [ViewAttributes](#viewattributes)
* [MethodAttributes](#methodattributes)

### DateTime - 时间

#### 构造函数

DateTime没有默认的构造函数(即没有new函数 )

提供一下构造函数:

* now()<br>
当前UTC时间

* nowMonotonic()<br>
当前自操作系统启动后的时间长度，仅用来标记时间间隔长度等，不能用作时间戳


#### 成员变量

N/A

#### 成员方法

* \_\_tostring <br> 字符串化当前时间，例如 2019/08/23 12:08:36.219.213.222
* toUnixTime()<br> 转换为Unix时间戳 (uint64数据)
* FromUnitTime(time | int64) <br> 从Unix时间戳转换到当前结构
* toStruct() <br> 转换成为DateTimeStruct结构，方便访问具体的时间属性（年月日等等）
* fromStruct() <br> 从DateTimeStruct转换成为当前结构
* localTimeUtcOffset() <br> 本地时间和UTC时间的间隔秒数


### DateTimeStruct - 时间结构对象

#### 构造函数

* new() <br> 默认构造，提供初始化一个默认的时间结构体对象
* new(DateTime now) <br> 从时间对象构造一个时间结构体对象


#### 成员变量

* nanoSec
* microSec
* milliSec
* sec
* min
* hour
* day
* month
* year

### Variant - 变量数值对象

#### 构造函数

提供默认的构造函数，支持boolean, 整数（int64)，浮点(double), 字符串。也支持指定的数据类型，如Variant.uint8(10),Variant.datatime(datetime_obj)等等

* new(boolean)
* new(integer)
* new(numeric)
* new(string)
* int8(integer)
* uint8(integer)
* int16(integer)
* uint16(integer)
* int32(integer)
* uint32(integer)
* int64(integer)
* uint64(integer)
* datetime(DateTime)

#### 成员函数

* isEmpty() <br> 是否是一个空对象
* isScalar() <br> 是否是元数据（非数组类型）
* isNumberic() <br> 是否是数值数据
* hasScalarType(type: number) <br> 是否含有指定的数据类型
* hasArrayType(type: number) <br> 是否含有制定的数组类型
* setScalar(val: any, type: number) <br> 设定元数据
* setScalarCopy(val: any, type: number) <br> 设定并复制数据
* setArray(array: array, arraySize: number, type: number) <br> 设定数组数据
* setArrayCopy(array: array, arraySize: number, type: number) <br> 设定数组数据
* copyRange()
* setRange()
* setRangeCopy()
* asLong() <br> 获取整数数据（尝试进行数据转换)
* asDouble() <br> 获取浮点数据 （尝试进行数据转换)
* asString() <br> 获取字符串数据
* asDateTime() <br> 获取时间戳
* asValue <br> 返回对应的Lua数据（可以是整数、浮点、字符串）， 失败返回nil, error


### DataValue - 数据数值

#### 构造函数

只提供一种默认的构造函数，即使用Variant作为输入

* new(val | Variant)

#### 成员变量

* hasValue (Readonly) <br> 是否有合法的数值，只读
* hasStatus (Readonly) <br> 是否有状态值，只读
* hasSourceTimestamp (Readonly) <br> 是否有数据源时间戳， 只读
* hasServerTimestamp (Readonly) <br> 是否有服务器时间戳，只读
* hasSorucePicoseconds (Readonly) 
* hasServerPicoseconds (Readonly)
* value -- Variant <br> 数据
* status -- StatusCode <br> 状态
* sourceTimestamp -- DateTime <br> 数据源时间戳
* sourcePicoseconds -- int16
* serverTimestamp -- DateTime <br> 服务器时间戳
* serverPicoseconds -- int16

### NodeId - 节点ID

#### 构造函数

* new(ns | integer, val | integer) <br> 使用整数作为ID
* new(ns | integer, val | string) <br> 使用字符串作为ID
* new(ns | integer, val | Guid) <br> 使用UUID作为字符串

#### 成员变量

* ns <br> 名字空间的ID
* type <br> ID类型
* index <br> ID数值（可以是整数、字符串以及UUID)

#### 成员函数

* isNull() <br> 是否为空
* hash() <br> 哈希


### ExpandedNodeId - 扩展的节点ID

#### 构造函数

* new(ns | integer, val | integer)
* new(ns | integer, val | string)

#### 成员变量

* nodeId
* namespaceUri
* serverIndex


### QualifiedName

#### 构造函数

* new(ns | integers, val | string)

#### 成员变量

* name
* namespaceIndex


### LocalizedText

#### 构造函数

* new(locale | string, text | string)

#### 成员变量

* text
* locale


### ObjectAttributes

#### 构造函数

* new()

#### 成员变量

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* eventNotifier

### VariableAttributes

#### 构造函数

* new()

#### 成员变量

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* value -- Variant
* dataType -- NodeId
* valueRank
* arrayDimensionsSize
* accessLevel
* userAccesssLevel
* minimumSamplingInterval
* historizing


### ViewAttributes

#### 构造函数

* new()

#### 成员变量

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* containsNoLoops
* eventNotifier


### MethodAttributes

#### 构造函数

* new()

#### 成员变量

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* executable
* userExecutable


