#include "entity_prefabs.h"
#include "components.h"
#include "config.h"
#include "display.h"
#include "game.h"
#include "linalg.h"
#include "map.h"
#include "texture_registry.h"


Entity entity_create_fren(World *world, Vec2 position) {
	Entity entity = entity_create(world);

	entity_add_position(world, entity, position);
	entity_add_randomwalk(world, entity);

	SpriteId sprite = sprite_create(TEXTURE_FREN, position, VEC2_ZERO);
	entity_add_spriteid(world, entity, sprite);
	sprite_set_layer(sprite, RENDER_LAYER_WALL);


	return entity;
}


Entity entity_create_torch(World *world, Vec2i tile, int intensity) {
	Entity entity = entity_create(world);

	Vec2 position = tile_to_world_coord(tile);

	entity_add_position(world, entity, position);

	SpriteId sprite = sprite_create(TEXTURE_TORCH, position, VEC2_ZERO);
	entity_add_spriteid(world, entity, sprite);
	sprite_set_layer(sprite, RENDER_LAYER_WALL);

	entity_add_light(world, entity, (Light){.intensity = intensity});
	map_set_light_source(game.map, tile.x, tile.y, 16);

	return entity;
}


Entity entity_create_furniture(World *world, Vec2i tile, TextureId texture) {
	Entity entity = entity_create(world);

	Vec2 position = tile_to_world_coord(tile);

	entity_add_position(world, entity, position);

	SpriteId sprite = sprite_create(texture, position, VEC2_ZERO);
	entity_add_spriteid(world, entity, sprite);
	// sprite_set_layer(sprite, RENDER_LAYER_WALL);

	return entity;
}

