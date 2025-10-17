#include "game.h"
#include "assets.h"
#include "collision.h"
#include "config.h"
#include "display.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "worldgen.h"
#include <raylib.h>
#include "debug.h"
#include <stdbool.h>

Game game = {0};
RenderTexture2D render_target;

void game_init() {
	physics_init();
	display_init();
	assets_init();
	// DrawTexture(texture, x * TILE_PIXEL_SIZE, y * TILE_PIXEL_SIZE, WHITE);

	game.map = map_create();

	int player_y = MAP_HEIGHT / 2;
	game.player = player_create(MAP_WIDTH / 2, player_y);

	render_target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

	// ColliderId plane = collider_create(
	// 	1, 
	// 	(Vec2){
	// 		.x = MAP_WIDTH * TILE_PIXEL_SIZE / 2.0f, .y = (player_y + 1.5) * TILE_PIXEL_SIZE
	// 	}, 
	// 	(Vec2){
	// 		.x = 500,
	// 		.y = TILE_PIXEL_SIZE / 2.
	// 	});
	// collider_get(plane)->debug = true;
}

void game_destroy() {
	map_destroy(game.map);

	assets_destroy();
	display_destroy();
	physics_destroy();
}

void game_update() {
	game.tick++;
	debug_start();
	input_update();
	player_update(&game.player);
}

static const Rectangle RENDER_SOURCE = {
	.x = 0, .y = 0, .width = WINDOW_WIDTH, .height = -WINDOW_HEIGHT
};
static const Rectangle RENDER_DEST = {
	.x = 0, .y = 0, .width = TARGET_WINDOW_WIDTH, .height = TARGET_WINDOW_HEIGHT
};
void game_draw() {
	player_render(&game.player);

	BeginTextureMode(render_target);
	ClearBackground(BLACK);
	display_render();
	EndTextureMode();

	DrawTexturePro(render_target.texture, RENDER_SOURCE, RENDER_DEST, (Vector2){0}, 0, WHITE);
	debug_end();
}
