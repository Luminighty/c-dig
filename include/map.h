#ifndef MAP_H
#define MAP_H

#include "config.h"
#include "tile.h"
#include <stdbool.h>


#define MAP_WIDTH (SCREEN_WIDTH * 2)
#define MAP_HEIGHT (SCREEN_HEIGHT * 2)


typedef struct {
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;


Map* map_create();
void map_destroy(Map* map);

void map_render(Map* map);
void map_dig(Map* map, int x, int y);

#define map_get(map, x, y) ((map)->tiles[(int)(y)][(int)(x)])
#define map_set(map, x, y, tile) (map)->tiles[(int)(y)][(int)(x)] = (tile)


#endif // MAP_H
