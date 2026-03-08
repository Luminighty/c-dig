#include "map.h"
#include "config.h"
#include "linalg.h"
#include "tile.h"
#include "worldgen.h"
#include <math.h>
#include <stdlib.h>
#include <memory.h>


#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

#define is_inside_map(x, y) ((x) >= 0 && (x) < MAP_WIDTH && (y) >= 0 && (y) < MAP_HEIGHT)


inline Tile map_get(Map* map, int x, int y) {
	return is_inside_map(x, y) ? map->tiles[y][x] : TILE_NONE;
}

inline void map_set(Map* map, int x, int y, Tile tile) {
	if (is_inside_map(x, y))
		map->tiles[y][x] = tile;
}

inline Vec2i world_to_tile_coord(Vec2 world) {
	return (Vec2i){
		.x = floor(world.x / TILE_PIXEL_SIZE),
		.y = floor(world.y / TILE_PIXEL_SIZE),
	};
}

inline Vec2 tile_to_world_coord(Vec2i tile) {
	return (Vec2){
		.x = tile.x * TILE_PIXEL_SIZE,
		.y = tile.y * TILE_PIXEL_SIZE,
	};
}

Map* map_create() {
	Map* map = malloc(sizeof(Map));
	memset(map, 0, sizeof(Map));
	foreach(x, y)
		map->tiles[y][x] = TILE_STONE;
	worldgen_run(map, 0);
	return map;
}

void map_destroy(Map* map) {
	free(map);
}

