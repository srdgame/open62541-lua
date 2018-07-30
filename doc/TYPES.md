## OPCUA Types

### OPCUA DateTime class

#### Constructor
No default public constructor

* now()
The current time in UTC time

* nowMonotonic()
CPU clock invariant to system time changes. Use only to measure durations, not absolute time.


#### Member variables

N/A

#### Methods

* \_\_tostring

* toUnixTime()
Convert to unix time in int64.

* FromUnitTime(time | int64)
From unix time.

* toStruct()
Convert to DateTimeStruct

* localTimeUtcOffset()
Offset between local time and UTC time


### OPCUA DateTimeStruct class

#### Constructors

* new()
Default constructor

* new(DateTime now)
Construct struct with datetime

#### Member variables

* nanoSec
* microSec
* milliSec
* sec
* min
* hour
* day
* month
* year

### OPCUA Variant class

#### Constructors

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

#### Methods

* isEmpty()

* isScalar()

* hasScalarType()

* hasArrayType()

* setScalar()

* setScalarCopy()

* setArray()

* setArrayCopy()

* copyRange()

* setRange()

* setRangeCopy()

* asLong()

* asDouble()

* asString()

* asDateTime()


### OPCUA DataValue class

#### Constructors

* new(val | Variant)

#### Member Variables

* hasValue (Readonly)
* hasStatus (Readonly)
* hasSourceTimestamp (Readonly)
* hasServerTimestamp (Readonly)
* hasSorucePicoseconds (Readonly)
* hasServerPicoseconds (Readonly)
* value -- Variant
* status -- StatusCode
* sourceTimestamp -- DateTime
* sourcePicoseconds -- int16
* serverTimestamp -- DateTime
* serverPicoseconds -- int16

### OPCUA NodeId class

#### Constructors

* new(ns | integer, val | integer)
* new(ns | integer, val | string)
* new(ns | integer, val | Guid)

#### Member Variables

* ns
Namespace index

* type
Identifier type

* index
Return the index

#### Methods

* isNull()
* hash()


### OPCUA ExpandedNodeId class

#### Constructors

* new(ns | integer, val | integer)
* new(ns | integer, val | string)

#### Member Variables

* nodeId
* namespaceUri
* serverIndex


### OPCUA QualifiedName class

#### Constructors

* new(ns | integers, val | string)

#### Member Variables

* name
* namespaceIndex


### OPCUA LocalizedText class

#### Constructors

* new(locale | string, text | string)

#### Member Variables

* text
* locale


### OPCUA ObjectAttributes class

#### Constructors

* new()

#### Member Variables

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* eventNotifier

### OPCUA VariableAttributes

#### Constructors

* new()

#### Member Variables

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


### OPCUA ViewAttributes

#### Constructors

* new()

#### Member Variables

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* containsNoLoops
* eventNotifier


### OPCUA MethodAttributes

#### Constructors

* new()

#### Member Variables

* specifiledAttributes
* displayName
* description
* writeMask
* userWriteMask
* executable
* userExecutable


