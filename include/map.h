#ifndef MAP_H
#define MAP_H

#include "config.h"
#include "linalg.h"
#include "tile.h"
#include <stdbool.h>
#include <stdint.h>


typedef uint8_t LightLevel;

typedef struct {
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
	LightLevel light[MAP_HEIGHT][MAP_WIDTH];
	LightLevel light_sources[MAP_HEIGHT][MAP_WIDTH];
	Rect2i dirty;
} Map;


Map* map_create();
void map_destroy(Map* map);

void map_dig(Map* map, int x, int y);

Tile map_get(Map* map, int x, int y);
void map_set(Map* map, int x, int y, Tile tile);
uint8_t map_get_light_level(Map* map, int x, int y);

void map_set_dirty(Map* map, int x, int y);
void map_set_dirty_rect(Map* map, Rect2i rect);
bool map_is_light_source(Map* map, int x, int y);
void map_add_light_source(Map* map, int x, int y, uint8_t intensity);
void map_remove_light_source(Map* map, int x, int y);
void map_update_light(Map* map);

Vec2i world_to_tile_coord(Vec2 world);
Vec2 tile_to_world_coord(Vec2i tile);


#endif // MAP_H
