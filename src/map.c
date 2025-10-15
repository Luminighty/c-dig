#include "map.h"
#include "tile.h"
#include "worldgen.h"
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

