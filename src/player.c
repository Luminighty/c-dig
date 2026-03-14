#include "player.h"
#include "entity_prefabs.h"
#include "texture_registry.h"
#include "components.h"
#include "physics.h"
#include "config.h"
#include "display.h"
#include "entity.h"
#include "game.h"
#include "input.h"
#include "map.h"
#include "linalg.h"
#include "random.h"
#include "tile.h"
#include "debug.h"


static inline PlayerToolSlot *add_tool(Player *player, PlayerToolKind kind, int amount) {
	PlayerToolSlot *slot = &player->tools[player->tools_count++];
	slot->amount = amount;
	slot->kind = kind;
	slot->unlimited = false;
	return slot;
}


Player player_create(World *world, int x, int y) {
	Entity entity = entity_create(world);

	Vec2 position = tile_to_world_coord((Vec2i){.x=x, .y=y});
	entity_add_position(world, entity, position);

	ColliderId collider = collider_create(entity, position, (Vec2){.x = 3, .y = 5});
	entity_add_rigidbody(world, entity, (Rigidbody){
		.id = collider,
		.gravity = 0.1f,
	});

	Vec2 sprite_offset = (Vec2){.x = -8, .y = -11};


	Player player = {
		.entity = entity,
		.movement = {.speed = 1.5f},
		.dig = { .speed = 100 },
		.collider = collider,
		.is_facing_right = true,
	};

	PlayerToolSlot *pickaxe = add_tool(&player, PLAYER_TOOL_PICKAXE, 0);
	pickaxe->unlimited = true;
	add_tool(&player, PLAYER_TOOL_WOOD, 64);
	add_tool(&player, PLAYER_TOOL_STONE, 64);
	add_tool(&player, PLAYER_TOOL_TORCH, 64);
	add_tool(&player, PLAYER_TOOL_CHEST, 64);
	add_tool(&player, PLAYER_TOOL_ANVIL, 64);

	TextureId body = TEXTURE_MINER_BODY_0000;
	TextureId cloth = TEXTURE_MINER_CLOTH_0001;
	TextureId beard = TEXTURE_MINER_BEARD_0001;
	TextureId hat = TEXTURE_MINER_HAT_0003;

	player.sprite = sprite_create(body, position, sprite_offset);
	entity_add_spriteid(world, entity, player.sprite);
	player.sprites[PLAYER_SPRITE_SLOT_BODY] = player.sprite;
	player.sprites[PLAYER_SPRITE_SLOT_CLOTH] = sprite_create(cloth, VEC2_ZERO, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_BEARD] = sprite_create(beard, VEC2_ZERO, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_HAT] = sprite_create(hat, VEC2_ZERO, sprite_offset);
	sprite_set_parent(player.sprites[PLAYER_SPRITE_SLOT_CLOTH], player.sprite);
	sprite_set_parent(player.sprites[PLAYER_SPRITE_SLOT_BEARD], player.sprite);
	sprite_set_parent(player.sprites[PLAYER_SPRITE_SLOT_HAT], player.sprite);

	sprite_set_layer(player.sprite, RENDER_LAYER_PLAYER);
	sprite_set_layer(player.sprites[PLAYER_SPRITE_SLOT_CLOTH], RENDER_LAYER_PLAYER);
	sprite_set_layer(player.sprites[PLAYER_SPRITE_SLOT_BEARD], RENDER_LAYER_PLAYER);
	sprite_set_layer(player.sprites[PLAYER_SPRITE_SLOT_HAT], RENDER_LAYER_PLAYER);

	world_print(world);

	return player;
}


static void randomize_sprite(Player* player) {
	TextureId body = random_range(TEXTURE_MINER_BODY_0000, TEXTURE_MINER_BODY_0005 + 1);
	TextureId cloth = random_range(TEXTURE_MINER_CLOTH_0000, TEXTURE_MINER_CLOTH_0005 + 1);
	TextureId beard = random_range(TEXTURE_MINER_BEARD_0000, TEXTURE_MINER_BEARD_0005 + 1);
	TextureId hat = random_range(TEXTURE_MINER_HAT_0000, TEXTURE_MINER_HAT_0005 + 1);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_BODY], body);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_CLOTH], cloth);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_BEARD], beard);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_HAT], hat);
}


static inline void player_movement(Game *game, Player* player) {
	Rigidbody *rb = entity_get_rigidbody(&game->world, player->entity);
	if (input.jump && rb->on_ground) {
		rb->velocity.y = -2;
		rb->on_ground = false;
	}
	rb->velocity.x = input.movement.x * player->movement.speed;
}


// static inline void player_dig(Player* player) {
// 	if (!input.digging) {
// 		player->dig.target = 0;
// 		return;
// 	}
// 	Direction dir = vec2_into_dir(input.movement);
// 	if (!dir)
// 		return;
//
// 	Work* work = &player->dig.work;
// 	if (dir != player->dig.target || work_is_inactive(work)) {
// 		Vec2 tile_pos = vec2_add(player->position, dir_into_vec2(dir));
// 		Tile tile = map_get(game.map, tile_pos.x, tile_pos.y);
// 		player->dig.tile = tile;
// 		player->dig.target = dir;
// 		work_start(work, 100 + 200 * tile_get_hardness(tile));
// 	}
//
// 	WorkUnit progress = player->dig.speed;
// 	if (tile_is_unbreakable(player->dig.tile))
// 		progress = 0;
// 	if (!work_progress(work, progress))
// 		return;
// 	printf("DONE\n");
// 	work_print(work); printf("\n");
// 	MaterialComposition composition = get_tile_composition(player->dig.tile);
// 	material_composition_merge_into(&player->bag, &composition);
// 	material_composition_print(&player->bag);
// 	work_reset(work);
// 	Vec2 tile_pos = vec2_add(player->position, dir_into_vec2(dir));
// 	map_set(game.map, tile_pos.x, tile_pos.y, TILE_AIR);
// }


static inline Vec2i current_mouse_tile() {
	Vec2 world_mouse = vec2_add(camera_get_position(), input.mouse);
	Vec2i world_mouse_tile = world_to_tile_coord(world_mouse);
	return world_mouse_tile;
}



static inline void player_build(Game *game, Tile tile) {
	static const int DIG_SIZE = 3;
	Vec2i tile_position = current_mouse_tile();
	for (int i = 0; i < DIG_SIZE; i++)
	for (int j = 0; j < DIG_SIZE; j++) {
		int x = tile_position.x + i;
		int y = tile_position.y + j;
		if (!tile_is_unbreakable(map_get(game->map, x, y)))
			map_set(game->map, x, y, tile);
	}
}


static inline void player_use(Game *game, PlayerToolSlot *slot) {
	if (slot->amount <= 0 && !slot->unlimited)
		return;
	if (!slot->unlimited)
		slot->amount--;

	Vec2i tile = current_mouse_tile();
	switch (slot->kind) {
	case PLAYER_TOOL_NONE: break;
	case PLAYER_TOOL_PICKAXE:
		player_build(game, TILE_AIR); break;
	case PLAYER_TOOL_WOOD:
		player_build(game, TILE_WOOD);
		break;
	case PLAYER_TOOL_STONE:
		player_build(game, TILE_STONE);
		break;
	case PLAYER_TOOL_TORCH: {
		entity_create_torch(&game->world, tile, 16);
	} break;
	case PLAYER_TOOL_CHEST:
		entity_create_furniture(&game->world, tile, TEXTURE_FURNITURE_CHEST);
		break;
	case PLAYER_TOOL_FURNACE:
		entity_create_furniture(&game->world, tile, TEXTURE_FURNITURE_FURNACE);
		break;
	case PLAYER_TOOL_WORKBENCH:
		entity_create_furniture(&game->world, tile, TEXTURE_FURNITURE_WORKBENCH);
		break;
	case PLAYER_TOOL_ANVIL:
		entity_create_furniture(&game->world, tile, TEXTURE_FURNITURE_ANVIL);
		break;
	case PLAYER_TOOL_LADDER:
		entity_create_furniture(&game->world, tile, TEXTURE_FURNITURE_LADDER);
		break;
	}
}


void player_update(Game *game, Player* player) {
	player_movement(game, player);
	// player_dig(player);
	if (input.randomize_player)
		randomize_sprite(player);
	Vec2 world_mouse = vec2_add(camera_get_position(), input.mouse);
	Vec2i world_mouse_tile = world_to_tile_coord(world_mouse);

	for (int i = 0; i < PLAYER_TOOLBAR_SIZE; i++) {
		if (input.slot[i + 1])
			player->current_tool = i;
	}
	int current_facing = player->is_facing_right ? 1 : -1;
	if (current_facing * input.movement.x < 0) {
		player->is_facing_right = input.movement.x > 0;
		sprite_set_flip_x(player->sprite, !player->is_facing_right);
	}
	
	if (input.clicked) {
		player_use(game, &player->tools[player->current_tool]);
	}
}


void player_render(Game *game, Player* player) {
	static const int x = SCREEN_WIDTH * TILE_PIXEL_SIZE / 2;
	static const int y = SCREEN_HEIGHT * TILE_PIXEL_SIZE / 2;
	Position *position = entity_get_position(&game->world, player->entity);

	camera_set_position((Vec2){.x = position->x - x, .y = position->y - y});
	// sprite_set_position(player->sprite, *position);
}

