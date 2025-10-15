#ifndef TILE_H
#define TILE_H

#include "assets_registry.h"
#include "material.h"
#include <assert.h>


typedef enum {
	TILE_HARDNESS_1 = 1 << 0,
	TILE_HARDNESS_2 = 1 << 1,
	TILE_HARDNESS_4 = 1 << 2,
	TILE_HARDNESS_8 = 1 << 3,

	TILE_META_SOLID = 1 << 5,
	TILE_META_OPAQUE = 1 << 6,
	TILE_META_UNBREAKABLE = 1 << 7,
} TileMeta;


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

	TILE_SIZE,
} Tile;


extern TileMeta tile_meta[];
extern TextureId tile_textures[];

MaterialComposition get_tile_composition(Tile tile);

#define tile_is_solid(tile) (tile_meta[tile] & TILE_META_SOLID)
#define tile_is_opaque(tile) (tile_meta[tile] & TILE_META_OPAQUE)
#define tile_is_unbreakable(tile) (tile_meta[tile] & TILE_META_UNBREAKABLE)

#define TILE_HARDNESS_SHIFT 0
#define TILE_HARDNESS_BITS (TILE_HARDNESS_1 | TILE_HARDNESS_2 | TILE_HARDNESS_4 | TILE_HARDNESS_8)
#define tile_get_hardness(tile) ((tile_meta[tile] & TILE_HARDNESS_BITS) >> TILE_HARDNESS_SHIFT)


#endif // TILE_H
