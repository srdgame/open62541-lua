#include "open62541.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "opcua_enums.hpp"
#include "opcua_types.hpp"
#include "opcua_node.hpp"
#include "opcua_client.hpp"
#include "opcua_server.hpp"

namespace lua_opcua {
	typedef std::function<void (UA_LogLevel level, UA_LogCategory category, const char *msg)> LogStdFunction;
	LogStdFunction g_ua_logger = nullptr;
	/*
	sol::function g_ua_logger;// = nullptr;
	*/
	void setLogger(LogStdFunction logger) {
		g_ua_logger = logger;
	}
	void UA_LUA_Logger(UA_LogLevel level, UA_LogCategory category, const char *msg, va_list args) {
		if (g_ua_logger && level > UA_LOGLEVEL_INFO) {
			char *buf = new char[2048];
			memset(buf, 0, 2048);
			vsprintf(buf, msg, args);
			g_ua_logger(level, category, buf);
			delete[] buf;
		} else {
			UA_Log_Stdout(level, category, msg, args);
		}
	}

	sol::table open_opcua(sol::this_state L) {
		sol::state_view lua(L);
		sol::table module = lua.create_table();

		reg_opcua_enums(module);
		reg_opcua_types(module);
		reg_opcua_node(module);
		reg_opcua_client(module);
		reg_opcua_server(module);

		//module.set_function("setLogger", [&](sol::function logger) { g_ua_logger = logger; });
		module.set_function("setLogger", setLogger);

		return module;
	}
}

extern "C" int luaopen_opcua(lua_State *L) {
	return sol::stack::call_lua(L, 1, lua_opcua::open_opcua);
}
