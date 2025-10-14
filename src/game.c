#include "game.h"
#include "collision.h"
#include "config.h"
#include "display.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "worldgen.h"
#include <raylib.h>
#include <stdbool.h>

Game game = {0};
RenderTexture2D render_target;

void game_init() {
	physics_init();
	game.map = map_create();
	worldgen_run(game.map, 0);
	game.player = player_create(MAP_WIDTH / 2, MAP_HEIGHT / 2);
	render_target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	collider_create(1, (Vec2){.x = 0, .y = 30}, (Vec2){.x = 100, .y = 1});
}

void game_destroy() {
	map_destroy(game.map);
	physics_deinit();
}

void game_update() {
	input_update();
	player_update(&game.player);
	game.tick++;
}

static const Rectangle RENDER_SOURCE = {
	.x = 0, .y = 0, .width = WINDOW_WIDTH, .height = -WINDOW_HEIGHT
};
static const Rectangle RENDER_DEST = {
	.x = 0, .y = 0, .width = TARGET_WINDOW_WIDTH, .height = TARGET_WINDOW_HEIGHT
};
void game_draw() {
	display_clear();
	map_render(game.map);
	player_render(&game.player);

	BeginTextureMode(render_target);
	display_render();
	EndTextureMode();

	DrawTexturePro(render_target.texture, RENDER_SOURCE, RENDER_DEST, (Vector2){0}, 0, WHITE);
}
