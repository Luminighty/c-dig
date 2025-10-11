#include "worldgen.h"
#include "map.h"
#include "tile.h"
#include <stdlib.h>

#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

#define draw_line(map, tile, from_x, from_y, condition, step)\
	for (int x = (from_x), y = (from_y); (condition); (step))\
		map->tiles[y][x] = tile;

#define fill(map, tile, from_x, from_y, to_x, to_y)\
	for (int y = (from_y); y < (to_y); y++)\
	for (int x = (from_x); x < (to_x); x++)\
		map->tiles[y][x] = tile;


static Tile generate_tile(int x, int y) {
	int val = rand() % 100;
	if (val > 98)
		return TILE_DIAMOND;
	if (val > 95)
		return TILE_IRON;
	if (val > 90)
		return TILE_COAL;
	if (val > 10)
		return TILE_STONE;
	return TILE_DIRT;
}

static void map_draw_border(Map* map) {
	draw_line(map, TILE_BEDROCK, 0, 0, (x < MAP_WIDTH), (x++));
	draw_line(map, TILE_BEDROCK, 0, 0, (y < MAP_HEIGHT), (y++));
	draw_line(map, TILE_BEDROCK, 0, MAP_HEIGHT - 1 , (x < MAP_WIDTH), (x++));
	draw_line(map, TILE_BEDROCK, MAP_WIDTH - 1, 0, (y < MAP_HEIGHT), (y++));
}

static void map_draw_spawn(Map* map) {
	static const int SPAWN_WIDTH = 2;
	static const int SPAWN_HEIGHT = 1;
	int cx = MAP_WIDTH / 2;
	int cy = MAP_HEIGHT / 2;

	fill(
		map,
		TILE_AIR,
		cx - SPAWN_WIDTH,
		cy - SPAWN_HEIGHT,
		cx + SPAWN_WIDTH,
		cy + SPAWN_HEIGHT
	);
}

static void worldgen_random(Map* map, uint64_t seed) {
	srand(seed);

	foreach(x, y) {
		Tile tile = generate_tile(x, y);
		map_set(map, x, y, tile);
	}
	map_draw_border(map);
	map_draw_spawn(map);

}

void worldgen_run(Map* map, uint64_t seed) {
	worldgen_random(map, seed);
}



