#include "tile.h"
#include "display.h"
#include "material.h"
#include "palette.h"
#include <stdbool.h>


Glyph tile_glyphs[TILE_SIZE] = {

[TILE_AIR] = glyph('.', PAL_GRAY(0x20), PAL_BLACK),
[TILE_BEDROCK] = glyph(0xB0, PAL_GRAY(0x20), PAL_BLACK),

[TILE_STONE] = glyph(0xDB, PAL_STONE, PAL_BLACK),
[TILE_DIRT] = glyph(0xB1, PAL_BROWN, PAL_DARK_BROWN),
[TILE_MOSS] = glyph('#', PAL_GREEN, PAL_STONE_DARK),


[TILE_CRYSTAL] = glyph(0xCE, PAL_PURPLE, PAL_BLACK),
[TILE_EMERALD] = glyph(0x09, PAL_GREEN, PAL_BLACK),
[TILE_AMETHYST] = glyph(0x09, PAL_BLUE, PAL_BLACK),
[TILE_RUBY] = glyph(0x09, PAL_RED, PAL_BLACK),

[TILE_FUNGAL] = glyph(0xB0, PAL_FUNGUS_2, PAL_FUNGUS),
[TILE_FUNGAL_MOSS] = glyph('#', PAL_GREEN, PAL_FUNGUS),
[TILE_FUNGAL_TRUNK] = glyph(0xBA, PAL_WHITE, PAL_BLACK),
[TILE_FUNGAL_CAP] = glyph(0xB2, PAL_RED, PAL_WHITE),

[TILE_ICE] = glyph(0x0F, PAL_BLUE, RGB(0x5, 0x10, 0xB2)),
[TILE_SNOW] = glyph(0xB0, PAL_BLUE, PAL_WHITE),

[TILE_MAGMA] = glyph(0xF7, PAL_RED, RGB(0x55, 0, 0)),
[TILE_HELLSTONE] = glyph(0x9D, PAL_RED, PAL_DARK_BROWN),

[TILE_VOID] = glyph(0xED, PAL_GRAY(0x33), PAL_BLACK),
[TILE_VOIDSTONE] = glyph(0x08, PAL_STONE, PAL_BLACK),

[TILE_STEAM] = glyph(0xB0, PAL_GRAY(0x40), PAL_BLACK),
[TILE_CHARCOAL] = glyph(0xB0, PAL_BLACK, PAL_GRAY(0x30)),

[TILE_GRASS] = glyph(0xB1, PAL_GREEN, PAL_DARK_BROWN),
[TILE_COAL] = glyph(0x0F, PAL_BLACK, PAL_STONE),
[TILE_IRON] = glyph(0x9C, PAL_RED, PAL_STONE),
[TILE_DIAMOND] = glyph(0x0F, PAL_BLUE, PAL_BLACK),

};

#define TILE_META_BLOCK TILE_META_SOLID | TILE_META_OPAQUE

#define TILE_HARDNESS(hardness) hardness << TILE_HARDNESS_SHIFT

TileMeta tile_meta[TILE_SIZE] = {
	[TILE_BEDROCK] = TILE_META_BLOCK | TILE_META_UNBREAKABLE,
	[TILE_AIR] = TILE_META_UNBREAKABLE,

	[TILE_STONE] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_DIRT] = TILE_META_BLOCK | TILE_HARDNESS(4),
	[TILE_MOSS] = TILE_META_BLOCK | TILE_HARDNESS(4),

	[TILE_CRYSTAL] = TILE_META_BLOCK | TILE_HARDNESS(10),
	[TILE_EMERALD] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_AMETHYST] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_RUBY] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_FUNGAL] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_FUNGAL_MOSS] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_FUNGAL_TRUNK] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_FUNGAL_CAP] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_ICE] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_SNOW] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_MAGMA] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_HELLSTONE] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_VOID] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_VOIDSTONE] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_STEAM] = TILE_META_BLOCK | TILE_HARDNESS(8),
	[TILE_CHARCOAL] = TILE_META_BLOCK | TILE_HARDNESS(8),

	[TILE_GRASS] = TILE_META_BLOCK | TILE_HARDNESS(4),
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
