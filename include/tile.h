#ifndef TILE_H
#define TILE_H

#include "assets_registry.h"
#include "config.h"
#include <assert.h>
#include <stdint.h>

typedef enum {
	TILE_FLAG_SOLID = 1 << 0,
	TILE_FLAG_OPAQUE = 1 << 1,
	TILE_FLAG_UNBREAKABLE = 1 << 2,
} TileFlag;


typedef struct {
	TextureId texture;
	uint8_t hardness;
	TileFlag flags;
	RenderLayer layer;
} TileData;


typedef enum {
	TILE_NONE,

	TILE_AIR,
	TILE_BEDROCK,

	TILE_STONE,
	TILE_MOSS,
	TILE_DIRT,

	TILE_GRASS,
	TILE_COAL,
	TILE_IRON,
	TILE_DIAMOND,

	TILE_EMERALD,
	TILE_AMETHYST,
	TILE_RUBY,

	TILE_WOOD,

	TILE_SIZE,
} Tile;


extern TileData tiles[TILE_SIZE];

#define tile_is_solid(tile)       (tiles[tile].flags & TILE_FLAG_SOLID)
#define tile_is_opaque(tile)      (tiles[tile].flags & TILE_FLAG_OPAQUE)
#define tile_is_unbreakable(tile) (tiles[tile].flags & TILE_FLAG_UNBREAKABLE)
#define tile_texture(tile)        (tiles[tile].texture)
#define tile_hardness(tile)       (tiles[tile].hardness)
#define tile_render_layer(tile)   (tiles[tile].layer)


#endif // TILE_H
