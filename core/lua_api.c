#include "lua_api.h"
#include "game.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>


static const char *LUA_MAIN = "./lua/main.lua";
static const char *LUA_CLIENT = "./lua/client.lua";

static const char *lua_callbacks[] = {
	[LUA_CALLBACK_INIT] = "OnInit",
	[LUA_CALLBACK_UPDATE] = "OnUpdate",
	[LUA_CALLBACK_UNLOAD] = "OnUnload",
	[LUA_CALLBACK_CLIENT_INIT] = "OnClientInit",
	[LUA_CALLBACK_CLIENT_UNLOAD] = "OnClientUnload",
	[LUA_CALLBACK_CLIENT_UI] = "OnClientUI",
	[LUA_CALLBACK_CLIENT_UPDATE] = "OnClientUpdate",
};

static inline void lua_load_file(LuaState *lua, const char *file) {
	if (luaL_dofile(lua->L, file) != LUA_OK) {
		fprintf(stderr, "Lua load Error: %s\n", lua_tostring(lua->L, -1));
		lua_pop(lua->L, 1);
		lua->loaded = false;
	}
}

void lua_add_global(LuaState *lua, const char *name, void* value) {
	lua_pushlightuserdata(lua->L, value);
	lua_setglobal(lua->L, name);
}

static inline void initialize_lua(LuaState *lua) {
	lua->loaded = true;
	luaL_openlibs(lua->L);

	lua_add_global(lua, "Game", &game);
	lua_add_global(lua, "World", game.world);

	lua_load_file(lua, LUA_MAIN);
}

LuaState lua_init() {
	lua_State *L = luaL_newstate();
	LuaState lua = { .L = L, .loaded = true };
	initialize_lua(&lua);
	lua_callback(&lua, LUA_CALLBACK_INIT);
	return lua;
}

void lua_client_init(LuaState *lua) {
	lua->is_client = true;
	if (luaL_dofile(lua->L, LUA_CLIENT) != LUA_OK) {
		fprintf(stderr, "Lua Error: %s\n", lua_tostring(lua->L, -1));
		lua_pop(lua->L, 1);
		lua->loaded = false;
	}
	lua_callback(lua, LUA_CALLBACK_CLIENT_INIT);
}

void lua_reload(LuaState *lua) {
	lua_callback(lua, LUA_CALLBACK_UNLOAD);
	if (lua->is_client)
		lua_callback(lua, LUA_CALLBACK_CLIENT_UNLOAD);

	lua_close(lua->L);
	lua->L = luaL_newstate();
	initialize_lua(lua);
	lua_callback(lua, LUA_CALLBACK_INIT);
	if (lua->is_client) {
		lua_load_file(lua, LUA_CLIENT);
		lua_callback(lua, LUA_CALLBACK_CLIENT_INIT);
	}
}

void lua_destroy(LuaState *lua) {
	lua_callback(lua, LUA_CALLBACK_UNLOAD);
	if (lua->is_client)
		lua_callback(lua, LUA_CALLBACK_CLIENT_UNLOAD);

	lua_close(lua->L);
	lua->L = NULL;
}

void lua_callback(LuaState *lua, LuaCallback callback) {
	if (!lua->loaded) return;
	lua_getglobal(lua->L, lua_callbacks[callback]);
	if (!lua_isfunction(lua->L, -1)) {
		lua_pop(lua->L, 1);
		return;
	}
	int result = lua_pcall(lua->L, 0, 0, 0);
	if (result != LUA_OK) {
		fprintf(stderr, "Lua Error: %s\n", lua_tostring(lua->L, -1));
		lua_pop(lua->L, 1);
	}
}

