#include "tile.h"
#include "assets_registry.h"
#include "material.h"
#include <stdbool.h>


TextureId tile_textures[] = {
	[TILE_BEDROCK] = TEXTURE_NONE,
	[TILE_AIR] = TEXTURE_TILE_STONE,

	[TILE_STONE] = TEXTURE_TILE_STONE,
	[TILE_DIRT] = TEXTURE_TILE_DIRT,
	[TILE_GRASS] = TEXTURE_TILE_GRASS,
	[TILE_MOSS] = TEXTURE_TILE_MOSS,

	[TILE_EMERALD] = TEXTURE_TILE_EMERALD,
	[TILE_AMETHYST] = TEXTURE_TILE_AMETHYST,
	[TILE_RUBY] = TEXTURE_TILE_RUBY,

	[TILE_COAL] = TEXTURE_TILE_COAL,
	[TILE_IRON] = TEXTURE_TILE_IRON,
	[TILE_DIAMOND] = TEXTURE_TILE_DIAMOND,
};


#define TILE_META_BLOCK TILE_META_SOLID | TILE_META_OPAQUE

#define TILE_HARDNESS(hardness) hardness << TILE_HARDNESS_SHIFT

TileMeta tile_meta[TILE_SIZE] = {
	[TILE_BEDROCK] = TILE_META_BLOCK | TILE_META_UNBREAKABLE,
	[TILE_AIR] = TILE_META_UNBREAKABLE,

	[TILE_STONE] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_DIRT] = TILE_META_BLOCK | TILE_HARDNESS(4),
	[TILE_GRASS] = TILE_META_BLOCK | TILE_HARDNESS(4),
	[TILE_MOSS] = TILE_META_BLOCK | TILE_HARDNESS(4),

	[TILE_EMERALD] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_AMETHYST] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_RUBY] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_COAL] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_IRON] = TILE_META_BLOCK | TILE_HARDNESS(10),
	[TILE_DIAMOND] = TILE_META_BLOCK | TILE_HARDNESS(12),
};


#define assert_hardness(offset, meta_value) \
	static_assert((1 << (TILE_HARDNESS_SHIFT + offset)) == meta_value, "Tile hardness shift is invalid!")

assert_hardness(0, TILE_HARDNESS_1);
assert_hardness(1, TILE_HARDNESS_2);
assert_hardness(2, TILE_HARDNESS_4);
assert_hardness(3, TILE_HARDNESS_8);

#undef assert_hardness


MaterialComposition get_tile_composition(Tile tile) {
	switch (tile) {
	case TILE_STONE:
		return material_composition_create(.stone = 10, .dirt = 3);
	case TILE_DIRT:
		return material_composition_create(.stone = 1, .dirt = 10);
	case TILE_COAL:
		return material_composition_create(.stone = 5, .coal = 5);
	case TILE_IRON:
		return material_composition_create(.stone = 5, .iron = 5);
	default:
		return (MaterialComposition){0};
	}
}
