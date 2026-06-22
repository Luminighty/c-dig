#include "entity_prefabs.h"
#include "components.h"
#include "config.h"
#include "display.h"
#include "game.h"
#include "linalg.h"
#include "map.h"
#include "random.h"
#include "texture_registry.h"

Entity entity_create_miner(World *world, Vec2 position) {
	Entity entity = entity_create(world);

	entity_add_position(world, entity, position);
	ColliderId collider = collider_create(entity, position, (Vec2){.x = 3, .y = 5});

	entity_add_rigidbody(world, entity, (Rigidbody){
		.id = collider,
		.gravity = 0.1f,
	});

	Vec2 sprite_offset = (Vec2){.x = -8, .y = -11};

	// TextureId text_body = TEXTURE_MINER_BODY_0000;
	// TextureId text_cloth = TEXTURE_MINER_CLOTH_0001;
	// TextureId text_beard = TEXTURE_MINER_BEARD_0001;
	// TextureId text_hat = TEXTURE_MINER_HAT_0003;
	TextureId text_body = random_range(TEXTURE_MINER_BODY_0000, TEXTURE_MINER_BODY_0005 + 1);
	TextureId text_cloth = random_range(TEXTURE_MINER_CLOTH_0000, TEXTURE_MINER_CLOTH_0005 + 1);
	TextureId text_beard = random_range(TEXTURE_MINER_BEARD_0000, TEXTURE_MINER_BEARD_0005 + 1);
	TextureId text_hat = random_range(TEXTURE_MINER_HAT_0000, TEXTURE_MINER_HAT_0005 + 1);

	SpriteId sprite_main = sprite_create(text_body, position, sprite_offset);
	SpriteId sprite_clothes = sprite_create(text_cloth, VEC2_ZERO, sprite_offset);
	SpriteId sprite_beard = sprite_create(text_beard, VEC2_ZERO, sprite_offset);
	SpriteId sprite_hat = sprite_create(text_hat, VEC2_ZERO, sprite_offset);

	sprite_set_parent(sprite_clothes, sprite_main);
	sprite_set_parent(sprite_beard, sprite_main);
	sprite_set_parent(sprite_hat, sprite_main);

	sprite_set_layer(sprite_main, RENDER_LAYER_PLAYER);
	sprite_set_layer(sprite_clothes, RENDER_LAYER_PLAYER);
	sprite_set_layer(sprite_beard, RENDER_LAYER_PLAYER);
	sprite_set_layer(sprite_hat, RENDER_LAYER_PLAYER);

	entity_add_spriteid(world, entity, sprite_main);

	return entity;
}

Entity entity_create_fren(World *world, Vec2 position) {
	Entity entity = entity_create(world);

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
	// sprite_set_layer(sprite, RENDER_LAYER_WALL);

	return entity;
}

