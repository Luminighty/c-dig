#include "map.h"
#include "config.h"
#include "darray.h"
#include "linalg.h"
#include "tile.h"
#include "worldgen.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>


#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

#define is_inside_map(x, y) ((x) >= 0 && (x) < MAP_WIDTH && (y) >= 0 && (y) < MAP_HEIGHT)


Tile map_get(Map* map, int x, int y) {
	return is_inside_map(x, y) ? map->tiles[y][x] : TILE_NONE;
}

void map_set(Map* map, int x, int y, Tile tile) {
	if (is_inside_map(x, y))
		map->tiles[y][x] = tile;
}

Vec2i world_to_tile_coord(Vec2 world) {
	return (Vec2i){
		.x = floor(world.x / TILE_PIXEL_SIZE),
		.y = floor(world.y / TILE_PIXEL_SIZE),
	};
}

Vec2 tile_to_world_coord(Vec2i tile) {
	return (Vec2){
		.x = tile.x * TILE_PIXEL_SIZE,
		.y = tile.y * TILE_PIXEL_SIZE,
	};
}
uint8_t map_get_light_level(Map* map, int x, int y) {
	if (is_inside_map(x, y))
		return map->light[y][x];
	return 0;
}

Map* map_create() {
	Map* map = malloc(sizeof(Map));
	memset(map, 0, sizeof(Map));
	foreach(x, y)
		map->tiles[y][x] = TILE_STONE;
	worldgen_run(map, rand());
	map_set_light_source(map, MAP_WIDTH / 2, MAP_HEIGHT / 2, 16);
	return map;
}


void map_destroy(Map* map) {
	free(map);
}

typedef struct {
	uint8_t intensity;
	int x;
	int y;
} LightFloodUnit;
DATypedef(LightFlood, LightFloodUnit);



void map_set_light_source(Map* map, int x, int y, uint8_t intensity) {
	if (!is_inside_map(x, y))
		return;
	// NOTE: This can be optimized by a lot, also this can crash atm welp
	LightFlood flood = {0};
	
	LightFloodUnit start = { intensity, x, y };
	da_push(flood, start);
	for (int i = 0; i < flood.count; i++) {
		LightFloodUnit *unit = &flood.items[i];
		if (!is_inside_map(unit->x, unit->y))
			continue;
		if (map->light[unit->y][unit->x] >= unit->intensity)
			continue;
		map->light[unit->y][unit->x] = unit->intensity;

		uint8_t intensity = unit->intensity;
		intensity = intensity > 1 ? intensity - 1 : 0;
		Tile tile = map_get(map, unit->x, unit->y);
		if (tile_is_opaque(tile))
			intensity = intensity > 4 ? intensity - 4 : 0;
		if (intensity == 0)
			continue;

		da_push(flood, ((LightFloodUnit){ intensity, unit->x - 1, unit->y }));
		da_push(flood, ((LightFloodUnit){ intensity, unit->x + 1, unit->y }));
		da_push(flood, ((LightFloodUnit){ intensity, unit->x ,    unit->y - 1 }));
		da_push(flood, ((LightFloodUnit){ intensity, unit->x ,    unit->y + 1 }));
	}
	// NOTE: Reuse this, just lazy now
	da_free(flood);
}

