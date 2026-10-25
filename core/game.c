#include "game.h"
#include "assets.h"
#include "config.h"
#include "display.h"
#include "entity.h"
#include "linalg.h"
#include "map.h"
#include "physics.h"
#include "random.h"
#include "systems.h"

Game game = {0};


void game_init() {
	random_init();

	assets_init();
	display_init();
	physics_init();

	game.systems = systems_create();
	game.map = map_create();
	game.world = world_create();

	systems_fire_init(&game.systems, game.world);
}


void game_destroy() {
	map_destroy(game.map);
	physics_destroy();
}


void game_draw() {
	systems_fire_draw(&game.systems, game.world);
}


void game_update() {
	game.tick++;
	// player_update(&game, &game.player);
	systems_fire_update(&game.systems, game.world);
	map_update_light(game.map);
}


Vec2 game_spawn_position() {
	return tile_to_world_coord((Vec2i){
		.x = MAP_WIDTH  / 2,
		.y = MAP_HEIGHT / 2
	});
}

