#ifndef LUA_API_H
#define LUA_API_H


#include <lua.h>
#include <stdbool.h>


typedef struct {
	bool loaded;
	bool is_client;
	lua_State *L;
} LuaState;


LuaState lua_init();
void lua_reload(LuaState *lua);
void lua_destroy(LuaState *lua);
void lua_client_init(LuaState *lua);

typedef enum {
	LUA_CALLBACK_INIT,
	LUA_CALLBACK_UPDATE,
	LUA_CALLBACK_UNLOAD,
	LUA_CALLBACK_CLIENT_INIT,
	LUA_CALLBACK_CLIENT_UNLOAD,
	LUA_CALLBACK_CLIENT_UPDATE,
	LUA_CALLBACK_CLIENT_UI,
} LuaCallback;

void lua_callback(LuaState *lua, LuaCallback callback);
void lua_add_global(LuaState *lua, const char *name, void* value);

#endif // LUA_API_H
