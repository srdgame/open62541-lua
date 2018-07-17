#pragma once
#include <iomanip>
//#include <iostream>

#include "open62541.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_interfaces.hpp"

namespace lua_opcua {

std::string toString(const UA_Guid& guid) {
	std::stringstream ss;
	ss << std::hex << std::setw(8) <<std::setfill('0')  << guid.data1;
	ss << std::hex << std::setw(4) <<std::setfill('0')  << guid.data2;
	ss << std::hex << std::setw(4) <<std::setfill('0')  << guid.data4;
	return ss.str();
}
std::string toString(const UA_NodeId& id) {
	std::stringstream ss;
	ss << "NodeId(ns=" << id.namespaceIndex << ";i=";
	switch(id.identifierType) {
		case UA_NODEIDTYPE_GUID:
			ss << toString(id.identifier.guid);
			break;
		case UA_NODEIDTYPE_STRING:
			ss << std::string((char*)id.identifier.string.data, id.identifier.string.length);
			break;
		case UA_NODEIDTYPE_BYTESTRING:
			ss << std::string((char*)id.identifier.byteString.data, id.identifier.byteString.length);
			break;
		case UA_NODEIDTYPE_NUMERIC:
		default:
			ss << id.identifier.numeric;
			break;
	}
	return ss.str();
}

static void
printNumber(unsigned short n, unsigned char *pos, size_t digits) {
    for(size_t i = digits; i > 0; --i) {
        pos[i-1] = (unsigned char)((n % 10) + '0');
        n = n / 10;
    }
}

UA_String
UA_DateTime_toString(UA_DateTime t) {
    /* length of the string is 31 (plus \0 at the end) */
    UA_String str = {31, (unsigned char*)UA_malloc(32)};
    if(!str.data)
        return UA_STRING_NULL;
    UA_DateTimeStruct tSt = UA_DateTime_toStruct(t);
    printNumber(tSt.month, str.data, 2);
    str.data[2] = '/';
    printNumber(tSt.day, &str.data[3], 2);
    str.data[5] = '/';
    printNumber(tSt.year, &str.data[6], 4);
    str.data[10] = ' ';
    printNumber(tSt.hour, &str.data[11], 2);
    str.data[13] = ':';
    printNumber(tSt.min, &str.data[14], 2);
    str.data[16] = ':';
    printNumber(tSt.sec, &str.data[17], 2);
    str.data[19] = '.';
    printNumber(tSt.milliSec, &str.data[20], 3);
    str.data[23] = '.';
    printNumber(tSt.microSec, &str.data[24], 3);
    str.data[27] = '.';
    printNumber(tSt.nanoSec, &str.data[28], 3);
    return str;
}

void reg_opcua_types(sol::table& module) {
	module.new_usertype<UA_DateTime>("DateTime",
		//"new", sol::factories([](void) { UA_DateTime date; return UA_DateTime_init(&date); }),
		//"__gc", sol::destructor(UA_DateTime_deleteMembers),
		"new", sol::no_constructor,
		"__tostring", [](const UA_DateTime& date) { return UA_DateTime_toString(date); },
		//"__tostring", [](const UA_DateTime& date) { std::stringstream ss; ss << date; return ss.str(); },
		"toUnixTime", [](const UA_DateTime& date) { return UA_DateTime_toUnixTime(date); },
		"fromUnixTime", [](UA_Int64 unixDate) { return UA_DateTime_fromUnixTime(unixDate); },
		"toStruct", [](const UA_DateTime& date) { return UA_DateTime_toStruct(date); },
		"now", [](void){ return UA_DateTime_now(); },
		"nowMonotonic", [](void){ return UA_DateTime_nowMonotonic(); },
		"localTimeUtcOffset", sol::property([](void) { return UA_DateTime_localTimeUtcOffset(); })
	);
	module.new_usertype<UA_DateTimeStruct>("DateTimeStruct",
		"nanoSec", &UA_DateTimeStruct::nanoSec,
		"microSec", &UA_DateTimeStruct::microSec,
		"milliSec", &UA_DateTimeStruct::milliSec,
		"sec", &UA_DateTimeStruct::sec,
		"min", &UA_DateTimeStruct::min,
		"hour", &UA_DateTimeStruct::hour,
		"day", &UA_DateTimeStruct::day,
		"month", &UA_DateTimeStruct::month,
		"year", &UA_DateTimeStruct::year
	);

	module.new_usertype<UA_Variant>("Variant",
		"new", sol::factories(
			[](bool val) {
				UA_Variant var;
				UA_Variant_init(&var);
				UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_BOOLEAN]);
				return var;
			},
			[](int64_t val) {
				UA_Variant var;
				UA_Variant_init(&var);
				UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_INT64]);
				return var;
			},
			[](double val) {
				UA_Variant var;
				UA_Variant_init(&var);
				UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_DOUBLE]);
				return var;
			},
			[](const char* val) {
				UA_String str = UA_STRING_ALLOC(val);
				UA_Variant var;
				UA_Variant_init(&var);
				UA_Variant_setScalarCopy(&var, &str, &UA_TYPES[UA_TYPES_STRING]);
				UA_String_deleteMembers(&str);
				return var;
			},
			[](const UA_DateTime& val) {
				std::cout << "UA_DateTime" << std::endl;
				UA_Variant var;
				UA_Variant_init(&var);
				UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_DATETIME]);
				return var;
			},
			[](const UA_Variant& obj) {
				UA_Variant var;
				UA_Variant_init(&var);
				UA_Variant_copy(&obj, &var);
				return var;
			},
			[]() {
				UA_Variant var;
				UA_Variant_init(&var);
				return var;
			}
		),
		"int8", sol::initializers([](UA_Variant& var, int8_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_SBYTE]); }),
		"uint8", sol::initializers([](UA_Variant& var, uint8_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_BYTE]);}),
		"int16", sol::initializers([](UA_Variant& var, int16_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_INT16]); }),
		"uint16", sol::initializers([](UA_Variant& var, uint16_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_UINT16]);}),
		"int32", sol::initializers([](UA_Variant& var, int32_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_INT32]); }),
		"uint32", sol::initializers([](UA_Variant& var, uint32_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_UINT32]);}),
		"int64", sol::initializers([](UA_Variant& var, int64_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_INT64]);}),
		"uint64", sol::initializers([](UA_Variant& var, uint64_t val) { UA_Variant_init(&var); UA_Variant_setScalarCopy(&var, &val, &UA_TYPES[UA_TYPES_UINT64]);}),

		"__gc", sol::destructor(UA_Variant_deleteMembers),
		"isEmpty", [](UA_Variant& var) { return UA_Variant_isEmpty(&var); },
		"isScalar", [](UA_Variant& var) { return UA_Variant_isScalar(&var); },
		"hasScalarType", [](UA_Variant& var, int type) { return UA_Variant_hasScalarType(&var, &UA_TYPES[type]); },
		"hasArrayType", [](UA_Variant& var, int type) { return UA_Variant_hasArrayType(&var, &UA_TYPES[type]); },
		"setScalar", [](UA_Variant& var, void* p, int type) { return UA_Variant_setScalar(&var, p, &UA_TYPES[type]); },
		"setScalarCopy", [](UA_Variant& var, void* p, int type) { return UA_Variant_setScalarCopy(&var, p, &UA_TYPES[type]); },
		"setArray", [](UA_Variant& var, void* array, size_t arraySize, int type) { return UA_Variant_setArray(&var, array, arraySize, &UA_TYPES[type]); },
		"setArrayCopy", [](UA_Variant& var, void* array, size_t arraySize, int type) { return UA_Variant_setArrayCopy(&var, array, arraySize, &UA_TYPES[type]); },
		"copyRange", [](UA_Variant& var, const UA_Variant& src, const UA_NumericRange range) { return UA_Variant_copyRange(&src, &var, range); },
		"setRange", [](UA_Variant& var, void* array, size_t arraySize, const UA_NumericRange range) { return UA_Variant_setRange(&var, array, arraySize, range); },
		"setRangeCopy", [](UA_Variant& var, void* array, size_t arraySize, const UA_NumericRange range) { return UA_Variant_setRangeCopy(&var, array, arraySize, range); },
		"asLong", [](UA_Variant& var) {
			std::int64_t val = 0;
			if (!UA_Variant_isScalar(&var))
				return val;
			if (var.type == &UA_TYPES[UA_TYPES_BOOLEAN])
				val = *(UA_Boolean*)var.data ? 1 : 0;
			else if (var.type == &UA_TYPES[UA_TYPES_SBYTE])
				val = *(UA_SByte*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_BYTE])
				val = *(UA_Byte*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_INT32])
				val = *(UA_Int32*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_INT32])
				val = *(UA_UInt32*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_INT64])
				val = *(UA_Int64*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_UINT64])
				val = *(UA_UInt64*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_FLOAT])
				val = *(UA_Float*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_DOUBLE])
				val = *(UA_Double*)var.data;
			return val;
		},
		"asDouble", [](UA_Variant& var) {
			double val = 0;
			if (!UA_Variant_isScalar(&var))
				return val;
			if (var.type == &UA_TYPES[UA_TYPES_BOOLEAN])
				val = *(UA_Boolean*)var.data ? 1 : 0;
			else if (var.type == &UA_TYPES[UA_TYPES_SBYTE])
				val = *(UA_SByte*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_BYTE])
				val = *(UA_Byte*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_INT32])
				val = *(UA_Int32*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_INT32])
				val = *(UA_UInt32*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_INT64])
				val = *(UA_Int64*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_UINT64])
				val = *(UA_UInt64*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_FLOAT])
				val = *(UA_Float*)var.data;
			else if (var.type == &UA_TYPES[UA_TYPES_DOUBLE])
				val = *(UA_Double*)var.data;
			return val;
		},
		"asString", [](UA_Variant& var) {
			if (!UA_Variant_isScalar(&var))
				return std::string("Not Scalar Value");
			if (var.type == &UA_TYPES[UA_TYPES_STRING]) {
				UA_String str = *(UA_String*)var.data;
				return std::string((const char*)str.data, str.length);
			}
			return std::string("Not String Type Value");
		}
	);

	module.new_usertype<UA_DataValue>("DataValue",
		"new", sol::factories(
			[](const UA_Variant& val) {
				UA_DataValue var;
				UA_DataValue_init(&var);
				if (!UA_Variant_isEmpty(&val))
					var.hasValue = true;
				UA_Variant_copy(&val, &var.value);
				return var;
			}
		),
		"__gc", sol::destructor(UA_DataValue_deleteMembers),
		"hasValue", sol::property([](UA_DataValue& obj) { return obj.hasValue; }),
		"hasStatus", sol::property([](UA_DataValue& obj) { return obj.hasStatus; }),
		"hasSourceTimestamp", sol::property([](UA_DataValue& obj) { return obj.hasSourceTimestamp; }),
		"hasServerTimestamp", sol::property([](UA_DataValue& obj) { return obj.hasServerTimestamp; }),
		"hasSourcePicoseconds", sol::property([](UA_DataValue& obj) { return obj.hasSourcePicoseconds; }),
		"hasServerPicoseconds", sol::property([](UA_DataValue& obj) { return obj.hasServerPicoseconds; }),
		MAP_PROPERTY_HAS(UA_DataValue, UA_Variant, value, hasValue),
		"status", sol::property(
			[](UA_DataValue& obj) { return obj.status; },
			[](UA_DataValue& obj, UA_StatusCode status) { obj.status = status, obj.hasStatus = true; }
		),
		MAP_PROPERTY_HAS(UA_DataValue, UA_DateTime, sourceTimestamp, hasSourceTimestamp),
		"sourcePicoseconds", sol::property(
			[](UA_DataValue& obj) { return obj.sourcePicoseconds; },
			[](UA_DataValue& obj, UA_UInt16 seconds) { obj.sourcePicoseconds = seconds, obj.hasSourcePicoseconds = true; }
		),
		MAP_PROPERTY_HAS(UA_DataValue, UA_DateTime, serverTimestamp, hasServerTimestamp),
		"serverPicoseconds", sol::property(
			[](UA_DataValue& obj) { return obj.serverPicoseconds; },
			[](UA_DataValue& obj, UA_UInt16 seconds) { obj.serverPicoseconds = seconds, obj.hasServerPicoseconds = true; }
		)
	),

	module.new_usertype<UA_NodeId>("NodeId",
		"new", sol::factories(
			[](int ns, int val) { return UA_NODEID_NUMERIC(ns, val); },
			[](int ns, const char* val){ return UA_NODEID_STRING_ALLOC(ns, val); },
			[](int ns, UA_Guid val){ UA_NODEID_GUID(ns, val); }
		),
		"__gc", sol::destructor(UA_NodeId_deleteMembers),
		"__eq", [](const UA_NodeId& left, const UA_NodeId& right) { return UA_NodeId_equal(&left, &right); },
		"isNull", [](const UA_NodeId& id) { return UA_NodeId_isNull(&id); },
		"hash", [](const UA_NodeId& id) { return UA_NodeId_hash(&id); },
		"ns", &UA_NodeId::namespaceIndex,
		"type", &UA_NodeId::identifierType,
		"index", sol::property([](UA_NodeId& id, sol::this_state L) {
			sol::variadic_results result;
			switch (id.identifierType) {
			case UA_NODEIDTYPE_NUMERIC:
				result.push_back({ L, sol::in_place_type<int>, id.identifier.numeric});
				break;
			case UA_NODEIDTYPE_STRING:
				result.push_back({ L, sol::in_place_type<const std::string>, std::string((char*)id.identifier.string.data, id.identifier.string.length)});
				break;
			case UA_NODEIDTYPE_GUID:
				result.push_back({ L, sol::in_place_type<UA_Guid>, id.identifier.guid});
				break;
			case UA_NODEIDTYPE_BYTESTRING:
				result.push_back({ L, sol::in_place_type<const std::string>, std::string((char*)id.identifier.byteString.data, id.identifier.byteString.length)});
				break;
			default:
				result.push_back({ L, sol::in_place_type<sol::lua_nil_t>, sol::lua_nil_t()});
				break;
			}
			return result;
		})
	);
	module.new_usertype<UA_QualifiedName>("QualifiedName",
		"new", sol::factories([](int ns, const char* val) { return UA_QUALIFIEDNAME_ALLOC(ns, val); }),
		"__gc", sol::destructor(UA_QualifiedName_deleteMembers),
		"__eq", [](const UA_QualifiedName& left, const UA_QualifiedName& right) {
			return left.namespaceIndex == right.namespaceIndex & UA_String_equal(&left.name, &right.name);
		},
		"__tostring", [](const UA_QualifiedName& obj) { 
			std::stringstream ss;
			ss << "QualifiedName(namespaceIndex=" << obj.namespaceIndex;
			ss << ";name=" << std::string((char*)obj.name.data, obj.name.length) << ")";
			return ss.str();
		},
		"name", sol::property([](const UA_QualifiedName& obj) { return std::string((char*)obj.name.data, obj.name.length); }),
		"namespaceIndex", sol::property([](const UA_QualifiedName& obj) { return obj.namespaceIndex; })
	);

	module.new_usertype<UA_LocalizedText>("LocalizedText",
		"new", sol::factories([](const char* locale, const char* text) { return UA_LOCALIZEDTEXT_ALLOC(locale, text); }),
		"__gc", sol::destructor(UA_LocalizedText_deleteMembers),
		"__tostring", [](const UA_LocalizedText& obj) {
			std::stringstream ss;
			ss << "LocalizedText(locale=" << std::string((char*)obj.locale.data, obj.locale.length);
			ss << ";text=" << std::string((char*)obj.text.data, obj.text.length) << ")";
			return ss.str();
		},
		"text", sol::property([](const UA_LocalizedText& obj) { return std::string((char*)obj.text.data, obj.text.length); }),
		"locale", sol::property([](const UA_LocalizedText& obj) { return std::string((char*)obj.locale.data, obj.locale.length); })
	);

	module.new_usertype<UA_ObjectAttributes>("ObjectAttributes",
		"new", sol::factories([]() { 
			return UA_ObjectAttributes_default;
		}),
		"__gc", sol::destructor(UA_ObjectAttributes_deleteMembers),
		"specifiedAttributes", &UA_ObjectAttributes::specifiedAttributes,
		MAP_PROPERTY(UA_ObjectAttributes, UA_LocalizedText, displayName),
		MAP_PROPERTY(UA_ObjectAttributes, UA_LocalizedText, description),
		"writeMask", &UA_ObjectAttributes::writeMask,
		"userWriteMask", &UA_ObjectAttributes::userWriteMask,
		"eventNotifier", &UA_ObjectAttributes::eventNotifier
	);

	module.new_usertype<UA_VariableAttributes>("VariableAttributes",
		"new", sol::factories([]() { 
			return UA_VariableAttributes_default;
		}),
		"__gc", sol::destructor(UA_VariableAttributes_deleteMembers),
		"specifiedAttributes", &UA_VariableAttributes::specifiedAttributes,
		MAP_PROPERTY(UA_VariableAttributes, UA_LocalizedText, displayName),
		MAP_PROPERTY(UA_VariableAttributes, UA_LocalizedText, description),
		"writeMask", &UA_VariableAttributes::writeMask,
		"userWriteMask", &UA_VariableAttributes::userWriteMask,
		MAP_PROPERTY(UA_VariableAttributes, UA_Variant, value),
		MAP_PROPERTY(UA_VariableAttributes, UA_NodeId, dataType),
		"valueRank", &UA_VariableAttributes::valueRank,
		"arrayDimensionsSize", &UA_VariableAttributes::arrayDimensionsSize,
		"accessLevel", &UA_VariableAttributes::accessLevel,
		"userAccessLevel", &UA_VariableAttributes::userAccessLevel,
		"minimumSamplingInterval", &UA_VariableAttributes::minimumSamplingInterval,
		"historizing", &UA_VariableAttributes::historizing
	);

	module.new_usertype<UA_ViewAttributes>("ViewAttributes",
		"new", sol::factories([]() { 
			return UA_ViewAttributes_default;
		}),
		"__gc", sol::destructor(UA_ViewAttributes_deleteMembers),
		"specifiedAttributes", &UA_ViewAttributes::specifiedAttributes,
		MAP_PROPERTY(UA_ViewAttributes, UA_LocalizedText, displayName),
		MAP_PROPERTY(UA_ViewAttributes, UA_LocalizedText, description),
		"writeMask", &UA_ViewAttributes::writeMask,
		"userWriteMask", &UA_ViewAttributes::userWriteMask,
		"containsNoLoops", &UA_ViewAttributes::containsNoLoops,
		"eventNotifier", &UA_ViewAttributes::eventNotifier
	);

	module.new_usertype<UA_MethodAttributes>("MethodAttributes",
		"new", sol::factories([]() { 
			return UA_MethodAttributes_default;
		}),
		"__gc", sol::destructor(UA_MethodAttributes_deleteMembers),
		"specifiedAttributes", &UA_MethodAttributes::specifiedAttributes,
		MAP_PROPERTY(UA_MethodAttributes, UA_LocalizedText, displayName),
		MAP_PROPERTY(UA_MethodAttributes, UA_LocalizedText, description),
		"writeMask", &UA_MethodAttributes::writeMask,
		"userWriteMask", &UA_MethodAttributes::userWriteMask,
		"executable", &UA_MethodAttributes::executable,
		"userExecutable", &UA_MethodAttributes::userExecutable
	);
}

}
