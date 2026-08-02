#include "game.h"
#include "assets.h"
#include "client.h"
#include "config.h"
#include "debug.h"
#include "display.h"
#include "entity.h"
#include "entity_prefabs.h"
#include "linalg.h"
#include "map.h"
#include "input.h"
#include "physics.h"
#include "player.h"
#include "random.h"
#include "systems.h"
#include "texture_registry.h"
#include "ui.h"
#include "ui_widgets.h"
#include <raylib.h>
#include <stdio.h>

Game game = {0};


void game_init() {
	random_init();

	if (!IS_HEADLESS) {
		// WARN: We may need to resize this during runtime, 
		// 	in case the window gets resized!
		Rectangle WINDOW = { .width = WINDOW_DIMENSION.x, .height = WINDOW_DIMENSION.y };
		int ui_width = WINDOW.width / UI_SCALE;
		int ui_height = WINDOW.height / UI_SCALE;
		game.render_target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
		game.ui_target = LoadRenderTexture(ui_width, ui_height);
	}

	assets_init();
	display_init();
	physics_init();

	game.systems = systems_create();
	game.map = map_create();
	game.world = world_create();

	// int player_y = MAP_HEIGHT / 2;
	// game.player = player_create(game.world, MAP_WIDTH / 2, player_y);

	Vec2i spawn_tile = {.x = MAP_WIDTH / 2, .y = MAP_HEIGHT / 2 };
	game.player = player_create(game.world, tile_to_world_coord(spawn_tile));

	// game.other_player = entity_create_miner(game.world, vec2_add(tile_to_world_coord(spawn_tile), (Vec2){.x = 10 }));
	// game.chest = entity_create_furniture(game.world, spawn_tile, TEXTURE_FURNITURE_CHEST);

	systems_fire_init(&game.systems, game.world);
}


void game_destroy() {
	map_destroy(game.map);
	physics_destroy();
}


void game_draw() {
	Rectangle WINDOW = {
		.width = WINDOW_DIMENSION.x, .height = WINDOW_DIMENSION.y 
	};
	Rectangle GAME_SOURCE = { .width = GAME_WIDTH, .height = -GAME_HEIGHT };
	Rectangle UI_SOURCE = { .width = (int)(WINDOW.width / UI_SCALE), .height = -(int)(WINDOW.height / UI_SCALE) };

	Rect2 _GAME_TARGET = game_viewport();
	Rectangle GAME_TARGET = {
		.x = _GAME_TARGET.x, .y = _GAME_TARGET.y,
		.width = _GAME_TARGET.w, .height = _GAME_TARGET.h,
	};
	player_render(&game, &game.player);
	systems_fire_draw(&game.systems, game.world);


	input.ui_hovering = false;
	BeginTextureMode(game.ui_target);
	ClearBackground(BLANK);
	// ui_render(&game.ui);
	EndTextureMode();

	BeginTextureMode(game.render_target);
	ClearBackground(BLACK);
	display_render();
	EndTextureMode();
	DrawTexturePro(game.render_target.texture, GAME_SOURCE, GAME_TARGET, (Vector2){0}, 0, WHITE);

	DrawTexturePro(game.ui_target.texture, UI_SOURCE, WINDOW, (Vector2){0}, 0, WHITE);

	debug_end();
	DrawFPS(10, 10);
}


void game_update() {
	game.tick++;
	debug_start();
	input_update();

	// WARN: This is just a basic hack to get mouse casting work out of the box
	//   At some point we probably need a better way to do this
	if (input.ui_hovering) {
		input.mouse_clicked = false;
	}
	player_update(&game, &game.player);
	systems_fire_update(&game.systems, game.world);
	map_update_light(game.map);
}

