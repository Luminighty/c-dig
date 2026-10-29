#include "entity_prefabs.h"
#include "components.h"
#include "config.h"
#include "display.h"
#include "entity_id.h"
#include "game.h"
#include "linalg.h"
#include "map.h"
#include "physics.h"
#include "texture_registry.h"

typedef enum {
	MINER_SPRITE_SLOT_CLOTH,
	MINER_SPRITE_SLOT_BEARD,
	MINER_SPRITE_SLOT_HAT,
} MinerSpriteSlot;


EntityPrefabPropsMiner entity_miner_get_props(World *world, Entity entity) {
	SpriteId *sprite = entity_get_spriteid(game.world, entity);
	SpriteId cloth   = sprite_socket_get(*sprite, MINER_SPRITE_SLOT_CLOTH);
	SpriteId beard   = sprite_socket_get(*sprite, MINER_SPRITE_SLOT_BEARD);
	SpriteId hat     = sprite_socket_get(*sprite, MINER_SPRITE_SLOT_HAT);

	Position *pos = entity_get_position(world, entity);

	return (EntityPrefabPropsMiner){
		.position = *pos,
		.body = sprite_get_texture(*sprite),
		.beard = sprite_get_texture(beard),
		.cloth = sprite_get_texture(cloth),
		.hat = sprite_get_texture(hat),
	};
}
Entity entity_create_miner(World *world, EntityPrefabPropsMiner props) {
	Entity entity = entity_create(world);
	entity_add_controllable(world, entity, (Controllable){0});

	entity_add_position(world, entity, props.position);
	ColliderId collider = collider_create(entity, props.position, (Vec2){.x = 3, .y = 5});

	entity_add_rigidbody(world, entity, (Rigidbody){
		.id = collider,
		.gravity = 0.1f,
	});

	Vec2 sprite_offset = (Vec2){.x = -8, .y = -11};
	SpriteId sprite_main    = sprite_create(props.body, props.position, sprite_offset);
	SpriteId sprite_clothes = sprite_create(props.cloth, VEC2_ZERO, sprite_offset);
	SpriteId sprite_beard   = sprite_create(props.beard, VEC2_ZERO, sprite_offset);
	SpriteId sprite_hat     = sprite_create(props.hat, VEC2_ZERO, sprite_offset);

	sprite_socket_add(sprite_main, MINER_SPRITE_SLOT_CLOTH, sprite_clothes);
	sprite_socket_add(sprite_main, MINER_SPRITE_SLOT_BEARD, sprite_beard);
	sprite_socket_add(sprite_main, MINER_SPRITE_SLOT_HAT, sprite_hat);

	sprite_set_layer(sprite_main, RENDER_LAYER_PLAYER);
	sprite_set_layer(sprite_clothes, RENDER_LAYER_PLAYER);
	sprite_set_layer(sprite_beard, RENDER_LAYER_PLAYER);
	sprite_set_layer(sprite_hat, RENDER_LAYER_PLAYER);

	entity_add_spriteid(world, entity, sprite_main);

	return entity;
}

Entity entity_create_spoopy_fren(World *world, Vec2 position, Entity target) {
	Entity entity = entity_create(world);
	entity_add_controllable(world, entity, (Controllable){0});

	entity_add_position(world, entity, position);
	entity_add_chase(world, entity, (Chase){.target = target});

	SpriteId sprite = sprite_create(TEXTURE_FREN, position, VEC2_ZERO);
	entity_add_spriteid(world, entity, sprite);
	sprite_set_layer(sprite, RENDER_LAYER_WALL);

	return entity;
}

Entity entity_create_fren(World *world, Vec2 position) {
	Entity entity = entity_create(world);
	entity_add_controllable(world, entity, (Controllable){0});

	entity_add_position(world, entity, position);
	entity_add_randomwalk(world, entity, (RandomWalk){.target = position });

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
	map_add_light_source(game.map, tile.x, tile.y, 32);

	return entity;
}


Entity entity_create_furniture(World *world, Vec2i tile, TextureId texture) {
	Entity entity = entity_create(world);

	Vec2 position = tile_to_world_coord(tile);

	entity_add_position(world, entity, position);

	SpriteId sprite = sprite_create(texture, position, VEC2_ZERO);
	entity_add_spriteid(world, entity, sprite);
	entity_add_controllable(world, entity, (Controllable){0});
	// sprite_set_layer(sprite, RENDER_LAYER_WALL);

	return entity;
}

