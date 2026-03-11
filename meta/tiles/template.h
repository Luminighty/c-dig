#include "texture_registry.h"
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


#define tile_is_solid(tile)       (tile_data[tile].flags & TILE_FLAG_SOLID)
#define tile_is_opaque(tile)      (tile_data[tile].flags & TILE_FLAG_OPAQUE)
#define tile_is_unbreakable(tile) (tile_data[tile].flags & TILE_FLAG_UNBREAKABLE)
#define tile_texture(tile)        (tile_data[tile].texture)
#define tile_hardness(tile)       (tile_data[tile].hardness)
#define tile_render_layer(tile)   (tile_data[tile].layer)
