#include "open62541.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_enums.hpp"
#include "opcua_types.hpp"
#include "opcua_node.hpp"
#include "opcua_client.hpp"
#include "opcua_server.hpp"

namespace lua_opcua {
	LogStdFunction UA_Client_Proxy::_logger = nullptr;
	LogStdFunction UA_Server_Proxy::_logger = nullptr;

	sol::table open_opcua(sol::this_state L) {
		sol::state_view lua(L);
		sol::table module = lua.create_table();

		reg_opcua_enums(module);
		reg_opcua_types(module);
		reg_opcua_node(module);
		reg_opcua_client(module);
		reg_opcua_server(module);

		return module;
	}
}

extern "C" int luaopen_opcua(lua_State *L) {
	return sol::stack::call_lua(L, 1, lua_opcua::open_opcua);
}
