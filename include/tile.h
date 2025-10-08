#ifndef TILE_H
#define TILE_H

#include "display.h"


typedef enum {
	TILE_META_UNBREAKABLE = 1 << 0,
	TILE_META_SOLID = 1 << 1,
	TILE_META_OPAQUE = 1 << 2,
} TileMeta;


typedef enum {
	TILE_AIR,
	TILE_BEDROCK,
	TILE_STONE,
	TILE_MOSS,
	TILE_DIRT,

	TILE_CRYSTAL,
	TILE_EMERALD,
	TILE_AMETHYST,
	TILE_RUBY,

	TILE_FUNGAL,
	TILE_FUNGAL_MOSS,
	TILE_FUNGAL_TRUNK,
	TILE_FUNGAL_CAP,

	TILE_ICE,
	TILE_SNOW,

	TILE_MAGMA,
	TILE_HELLSTONE,
	
	TILE_VOID,
	TILE_VOIDSTONE,
	TILE_STEAM,
	TILE_CHARCOAL,

	TILE_GRASS,
	TILE_COAL,
	TILE_IRON,
	TILE_DIAMOND,

	TILE_SIZE,
} Tile;


extern Glyph tile_glyphs[];
extern TileMeta tile_meta[];

#define tile_is_solid(tile) (tile_meta[tile] & TILE_META_SOLID)
#define tile_is_opaque(tile) (tile_meta[tile] & TILE_META_OPAQUE)
#define tile_is_unbreakable(tile) (tile_meta[tile] & TILE_META_UNBREAKABLE)


#endif // TILE_H
