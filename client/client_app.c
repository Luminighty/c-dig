#include "client/client_app.h"
#include "debug.h"
#include "entity_id.h"
#include "game.h"
#include "lua_api.h"

ClientApp client_app = {0};

void client_app_init() {
	client_app.player = player_create(game.world, ENTITY_NONE);

	Rectangle WINDOW = { .width = WINDOW_DIMENSION.x, .height = WINDOW_DIMENSION.y };
	int ui_width = WINDOW.width / UI_SCALE;
	int ui_height = WINDOW.height / UI_SCALE;
	client_app.render_target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
	client_app.ui_target = LoadRenderTexture(ui_width, ui_height);

	lua_add_global(&game.lua, "Player", &client_app.player);
	lua_client_init(&game.lua);
}

void client_app_update() {
	debug_start();
	input_update();

	if (input.ui_hovering) {
		input.mouse_clicked = false;
	}
	player_update(&game, &client_app.player);
	lua_callback(&game.lua, LUA_CALLBACK_CLIENT_UPDATE);

	if (input.hot_reload)
		lua_reload(&game.lua);
}

void client_app_render() {
	player_render(&game, &client_app.player);
	lua_callback(&game.lua, LUA_CALLBACK_CLIENT_UI);
}

void client_app_destroy() {

}

