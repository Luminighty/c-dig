#include "game.h"
#include "assets.h"
#include "debug.h"
#include "display.h"
#include "entity.h"
#include "map.h"
#include "input.h"
#include "physics.h"
#include "player.h"
#include "random.h"
#include "systems.h"
#include "ui.h"

Game game = {0};


void game_init() {
	random_init();

	game.render_target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

	assets_init();
	display_init();
	physics_init();

	game.systems = systems_create();
	game.map = map_create();
	game.world = world_create();
	int player_y = MAP_HEIGHT / 2;
	game.player = player_create(&game.world, MAP_WIDTH / 2, player_y);

	systems_fire_init(&game.systems, &game.world);
}


void game_destroy() {
	map_destroy(game.map);
	physics_destroy();
}


static const Rectangle RENDER_SOURCE = {
	.x = 0, .y = 0, .width = WINDOW_WIDTH, .height = -WINDOW_HEIGHT
};
static const Rectangle RENDER_DEST = {
	.x = 0, .y = 0, .width = TARGET_WINDOW_WIDTH, .height = TARGET_WINDOW_HEIGHT
};
void game_draw() {
	player_render(&game, &game.player);

	systems_fire_draw(&game.systems, &game.world);

	BeginTextureMode(game.render_target);
	ClearBackground(BLACK);
	display_render();
	EndTextureMode();

	DrawTexturePro(game.render_target.texture, RENDER_SOURCE, RENDER_DEST, (Vector2){0}, 0, WHITE);
	ui_render(&game.ui);
	debug_end();
}


void game_update() {
	game.tick++;
	debug_start();
	input_update();
	player_update(&game, &game.player);
	systems_fire_update(&game.systems, &game.world);
}

