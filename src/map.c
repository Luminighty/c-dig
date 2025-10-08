#include "map.h"
#include "config.h"
#include "display.h"
#include "game.h"
#include "tile.h"
#include <stdlib.h>
#include <memory.h>

static void map_draw_border(Map* map);
static void map_draw_spawn(Map* map);

#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

Map* map_create() {
	Map* map = malloc(sizeof(Map));
	memset(map, 0, sizeof(Map));
	foreach(x, y)
		map->tiles[y][x] = TILE_STONE;
	map_draw_border(map);
	map_draw_spawn(map);
	return map;
}

void map_destroy(Map* map) {
	free(map);
}


void map_render(Map *map) {
	for(int y = 0; y < SCREEN_HEIGHT; y++)
	for(int x = 0; x < SCREEN_WIDTH; x++) {
		int map_x = x + game.player.position.x - (SCREEN_WIDTH / 2);
		int map_y = y + game.player.position.y - (SCREEN_HEIGHT / 2);
		Tile t = map->tiles[map_y][map_x];
		Glyph g = tile_glyphs[t];
		display_putchar(x, y, g, 1);
	}
}


#define draw_line(map, tile, from_x, from_y, condition, step)\
	for (int x = (from_x), y = (from_y); (condition); (step))\
		map->tiles[y][x] = tile;

#define fill(map, tile, from_x, from_y, to_x, to_y)\
	for (int y = (from_y); y < (to_y); y++)\
	for (int x = (from_x); x < (to_x); x++)\
		map->tiles[y][x] = tile;


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

#undef fill
#undef draw_line


