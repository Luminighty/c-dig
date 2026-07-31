#include "map.h"
#include "config.h"
#include "darray.h"
#include "linalg.h"
#include "prioqueue.h"
#include "tile.h"
#include "worldgen.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

#define is_inside_map(x, y) ((x) >= 0 && (x) < MAP_WIDTH && (y) >= 0 && (y) < MAP_HEIGHT)


Tile map_get(Map* map, int x, int y) {
	return is_inside_map(x, y) ? map->tiles[y][x] : TILE__NONE;
}

void map_set(Map* map, int x, int y, Tile tile) {
	if (!is_inside_map(x, y))
		return;
	map->tiles[y][x] = tile;
	map_set_dirty(map, x, y);
}

Vec2i world_to_tile_coord(Vec2 world) {
	return (Vec2i){
		.x = world.x / TILE_PIXEL_SIZE,
		.y = world.y / TILE_PIXEL_SIZE,
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
	map_add_light_source(map, MAP_WIDTH / 2, MAP_HEIGHT / 2, 16);
	return map;
}


void map_destroy(Map* map) {
	free(map);
}

static inline void _map_set_dirty(Map* map, int x, int y) {
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= MAP_WIDTH) x = MAP_WIDTH - 1;
	if (y >= MAP_HEIGHT) y = MAP_HEIGHT - 1;

	if (map->dirty.w == 0 && map->dirty.h == 0) {
		map->dirty.x = x;
		map->dirty.y = y;
		map->dirty.w = 1;
		map->dirty.h = 1;
		return;
	}
	if (x < map->dirty.x) {
		map->dirty.w += map->dirty.x - x;
		map->dirty.x = x;
	}
	if (y < map->dirty.y) {
		map->dirty.h += map->dirty.y - y;
		map->dirty.y = y;
	}
	int right = map->dirty.x + map->dirty.w - 1;
	int bottom = map->dirty.y + map->dirty.h - 1;
	if (x > right)
		map->dirty.w += x - right;
	if (y > bottom)
		map->dirty.h += y - bottom;
}


void map_set_dirty(Map* map, int x, int y) {
	LightLevel intensity = map->light[y][x];
	_map_set_dirty(map, x - intensity, y - intensity);
	_map_set_dirty(map, x + intensity, y + intensity);
}

void map_set_dirty_rect(Map* map, Rect2i rect) {
	map_set_dirty(map, rect.x, rect.y);
	map_set_dirty(map, rect.x + rect.w, rect.y + rect.h);
}

typedef struct {
	uint8_t intensity;
	Vec2i tile;
} LightFloodUnit;

typedef PrioQueue(LightFloodUnit) LightFlood;

int cmp_floodunit(LightFloodUnit *a, LightFloodUnit *b) {
	if (a->intensity > b->intensity)
		return 1;
	if (a->intensity < b->intensity)
		return -1;
	return 0;
}
static LightFlood flood = prioqueue_create(cmp_floodunit);

static void run_light_flood(Map *map);

void map_update_light(Map* map) {
	if (map->dirty.w == 0 && map->dirty.h == 0) {
		run_light_flood(map);
		return;
	}
	// printf("Updating Light "RECT2I_FMT"...\n", RECT2I_ARG(map->dirty));

	// NOTE: Clear all the light in the dirty area
	// While also queueing up all the light sources
	// We need to do an additional step around the rect too
	RECT2I_FOREACH(map->dirty, x, y) {
		if (map_is_light_source(map, x, y)) {
			LightFloodUnit unit = { map->light_sources[y][x], (Vec2i){{x, y}} };
			prioqueue_push(&flood, unit);
			// printf("Light Source @%d,%d = %d...\n", x, y, map->light_sources[y][x]);
		} else {
			map->light[y][x] = 0;
		}
	}
	for (int x = map->dirty.x - 1; x <= map->dirty.x + map->dirty.w + 1; x++) {
		int top = map->dirty.y - 1;
		if (is_inside_map(x, top) && map->light[top][x] > 0) {
			LightFloodUnit unit = { map->light[top][x], (Vec2i){{x, top}} };
			prioqueue_push(&flood, unit);
			// printf("Border @%d,%d = %d...\n", x, top, map->light[top][x]);
		}
		int bottom = map->dirty.y + map->dirty.h + 1;
		if (is_inside_map(x, bottom) && map->light[bottom][x] > 0) {
			LightFloodUnit unit = { map->light[bottom][x], (Vec2i){{x, bottom}} };
			prioqueue_push(&flood, unit);
			// printf("Border @%d,%d = %d...\n", x, bottom, map->light[bottom][x]);
		}
	}
	for (int y = map->dirty.y - 1; y <= map->dirty.y + map->dirty.h + 1; y++) {
		int left = map->dirty.x - 1;
		if (is_inside_map(left, y) && map->light[y][left] > 1) {
			LightFloodUnit unit = { map->light[y][left], (Vec2i){{left, y}} };
			prioqueue_push(&flood, unit);
			// printf("Border @%d,%d = %d...\n", left, y, map->light[y][left]);
		}
		int right = map->dirty.x + map->dirty.w + 1;
		if (is_inside_map(right, y) && map->light[y][right] > 1) {
			LightFloodUnit unit = { map->light[y][right], (Vec2i){{right, y}} };
			prioqueue_push(&flood, unit);
			// printf("Border @%d,%d = %d...\n", right, y, map->light[y][right]);
		}
	}

	// printf("Flood %zu...\n", flood.count);
	run_light_flood(map);

	map->dirty.size = VEC2I_ZERO;
}


bool map_is_light_source(Map *map, int x, int y) {
	if (!is_inside_map(x, y))
		return false;
	return map->light_sources[y][x];
}

void map_remove_light_source(Map *map, int x, int y) {
	if (!is_inside_map(x, y))
		return;
	map->light_sources[y][x] = 0;
	map_set_dirty(map, x, y);
}

void map_add_light_source(Map *map, int x, int y, uint8_t intensity) {
	if (!is_inside_map(x, y))
		return;
	map->light_sources[y][x] = intensity;
	map_set_dirty(map, x, y);
}



static inline void push_lightflood_unit(Map *map, LightFlood *flood, LightFloodUnit unit) {
	if (!is_inside_map(unit.tile.x, unit.tile.y))
		return;
	if (map->light[unit.tile.y][unit.tile.x] >= unit.intensity)
		return;
	prioqueue_push(flood, unit);
}


// NOTE: This can cause flickering at the moment
// 	We need to do double buffering in order to fix it
// 	Another improvement would be to create a work thread with the double buffering
// 	But for smaller lights what we currently have is totally fine
#define LIGHT_FLOOD_LIMIT 20000
static void run_light_flood(Map *map) {
	size_t light_tiles_processed = 0;
	while (flood.count > 0) {
		if (light_tiles_processed >= LIGHT_FLOOD_LIMIT)
			break;
		LightFloodUnit unit = prioqueue_pop(&flood);
		light_tiles_processed++;
		if (map->light[unit.tile.y][unit.tile.x] > unit.intensity)
			continue;
		map->light[unit.tile.y][unit.tile.x] = unit.intensity;

		uint8_t intensity = unit.intensity;
		intensity = intensity > 1 ? intensity - 1 : 0;
		Tile tile = map_get(map, unit.tile.x, unit.tile.y);
		if (tile_is_opaque(tile))
			intensity = intensity > 4 ? intensity - 4 : 0;
		if (intensity == 0)
			continue;

		#define push_new_unit(map, flood, vec2) \
			push_lightflood_unit(map, flood, ((LightFloodUnit){ intensity, vec2i_add(unit.tile, vec2) }))

		push_new_unit(map, &flood, VEC2I_RIGHT);
		push_new_unit(map, &flood, VEC2I_LEFT);
		push_new_unit(map, &flood, VEC2I_UP);
		push_new_unit(map, &flood, VEC2I_DOWN);
	}
	// if (light_tiles_processed > 0)
	// 	printf("Light flooded %zu\n", light_tiles_processed);
}

