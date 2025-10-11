#include "map.h"
#include "config.h"
#include "display.h"
#include "game.h"
#include "palette.h"
#include "tile.h"
#include <stdlib.h>
#include <memory.h>


#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

#define is_inside_map(x, y) ((x) >= 0 && (x) < MAP_WIDTH && (y) >= 0 && (y) < MAP_HEIGHT)

Map* map_create() {
	Map* map = malloc(sizeof(Map));
	memset(map, 0, sizeof(Map));
	foreach(x, y)
		map->tiles[y][x] = TILE_STONE;
	return map;
}

void map_destroy(Map* map) {
	free(map);
}

static const Glyph tile_void = glyph(0xEA, PAL_GRAY(0x05), PAL_BLACK);

void map_render(Map *map) {
	for(int y = 0; y < SCREEN_HEIGHT; y++)
	for(int x = 0; x < SCREEN_WIDTH; x++) {
		int map_x = x + game.player.position.x - (SCREEN_WIDTH / 2);
		int map_y = y + game.player.position.y - (SCREEN_HEIGHT / 2);
		if (!is_inside_map(map_x, map_y)) {
			display_putchar(x, y, tile_void, 1);
		} else {
			Tile t = map->tiles[map_y][map_x];
			Glyph g = tile_glyphs[t];
			display_putchar(x, y, g, 1);
		}

	}
}

