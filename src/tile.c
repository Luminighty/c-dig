#include "tile.h"
#include "assets_registry.h"
#include "config.h"

#define BASE_FLAGS (TILE_FLAG_SOLID | TILE_FLAG_OPAQUE)
#define BASE_HARDNESS 64
#define BASE_LAYER RENDER_LAYER_NORMAL

TileData tiles[TILE_SIZE] = {
	[TILE_BEDROCK] = {
		.texture = TEXTURE_NONE,
		.flags = BASE_FLAGS | TILE_FLAG_UNBREAKABLE,
		.layer = BASE_LAYER,
	},
	[TILE_AIR] = {
		.texture = TEXTURE_TILE_STONE,
		.flags = TILE_FLAG_UNBREAKABLE,
		.layer = RENDER_LAYER_BACKGROUND,
	},
	[TILE_STONE] = {
		.texture = TEXTURE_TILE_STONE,
		.hardness = BASE_HARDNESS,
		.flags = BASE_FLAGS,
		.layer = BASE_LAYER,
	},
	[TILE_DIRT] = {
		.texture = TEXTURE_TILE_DIRT,
		.hardness = 20,
		.flags = BASE_FLAGS,
		.layer = BASE_LAYER,
	},
	[TILE_GRASS] = {
		.texture = TEXTURE_TILE_GRASS,
		.hardness = 20,
		.flags = BASE_FLAGS,
		.layer = BASE_LAYER,
	},
	[TILE_MOSS] = {
		.texture = TEXTURE_TILE_MOSS,
		.hardness = 20,
		.flags = BASE_FLAGS,
		.layer = BASE_LAYER,
	},
	[TILE_EMERALD] = {
		.texture = TEXTURE_TILE_EMERALD,
		.hardness = BASE_HARDNESS,
		.flags = BASE_FLAGS,
		.layer = BASE_LAYER,
	},
	[TILE_AMETHYST] = {
		.texture = TEXTURE_TILE_AMETHYST,
		.flags = BASE_FLAGS,
		.hardness = BASE_HARDNESS,
		.layer = BASE_LAYER,
	},
	[TILE_RUBY] = {
		.texture = TEXTURE_TILE_RUBY,
		.flags = BASE_FLAGS,
		.hardness = BASE_HARDNESS,
		.layer = BASE_LAYER,
	},
	[TILE_COAL] = {
		.texture = TEXTURE_TILE_COAL,
		.flags = BASE_FLAGS,
		.hardness = BASE_HARDNESS,
		.layer = BASE_LAYER,
	},
	[TILE_IRON] = {
		.texture = TEXTURE_TILE_IRON,
		.flags = BASE_FLAGS,
		.hardness = BASE_HARDNESS,
		.layer = BASE_LAYER,
	},
	[TILE_DIAMOND] = {
		.texture = TEXTURE_TILE_DIAMOND,
		.flags = BASE_FLAGS,
		.hardness = BASE_HARDNESS,
		.layer = BASE_LAYER,
	},
	[TILE_WOOD] = {
		.texture = TEXTURE_TILE_WOOD,
		.flags = BASE_FLAGS,
		.hardness = BASE_HARDNESS,
		.layer = BASE_LAYER,
	},
};

