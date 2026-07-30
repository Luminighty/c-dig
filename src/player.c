#include "player.h"
#include "entity_id.h"
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
#include "work.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>


static inline PlayerToolSlot *add_tool(PlayerController *player, PlayerToolKind kind, int amount) {
	assert(player->tools_count < PLAYER_TOOLBAR_SIZE);
	PlayerToolSlot *slot = &player->tools[player->tools_count++];
	slot->amount = amount;
	slot->kind = kind;
	slot->unlimited = false;
	return slot;
}

PlayerController player_create(World *world, Vec2 position) {
	Entity entity = entity_create_miner(world, position);

	PlayerController player = {
		.entity = entity,
		.movement = {.speed = 1.5f},
		.dig = { .speed = 100 },
		.is_facing_right = true,
	};

	PlayerToolSlot *pickaxe = add_tool(&player, PLAYER_TOOL_PICKAXE, 0);
	pickaxe->unlimited = true;
	add_tool(&player, PLAYER_TOOL_WOOD, 64);
	add_tool(&player, PLAYER_TOOL_STONE, 64);
	add_tool(&player, PLAYER_TOOL_TORCH, 64);
	// add_tool(&player, PLAYER_TOOL_CHEST, 64);
	// add_tool(&player, PLAYER_TOOL_ANVIL, 64);

	world_print(world);

	return player;
}


static void randomize_sprite(PlayerController* player) {
	// TODO: Reimplement this
	// The issue is that I do not store the slots anymore in the player controller
	// So this would have to be implemented on the entity level

	// TextureId body = random_range(TEXTURE_MINER_BODY_0000, TEXTURE_MINER_BODY_0005 + 1);
	// TextureId cloth = random_range(TEXTURE_MINER_CLOTH_0000, TEXTURE_MINER_CLOTH_0005 + 1);
	// TextureId beard = random_range(TEXTURE_MINER_BEARD_0000, TEXTURE_MINER_BEARD_0005 + 1);
	// TextureId hat = random_range(TEXTURE_MINER_HAT_0000, TEXTURE_MINER_HAT_0005 + 1);
	// sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_BODY], body);
	// sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_CLOTH], cloth);
	// sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_BEARD], beard);
	// sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_HAT], hat);
}


static inline void player_movement(Game *game, PlayerController* player) {
	if (entity_has_component(game->world, player->entity, COMPONENT_RIGIDBODY)) {
		Rigidbody *rb = entity_get_rigidbody(game->world, player->entity);
		if (input.jump && rb->on_ground) {
			rb->velocity.y = -2;
			rb->on_ground = false;
		}
		rb->velocity.x = input.movement.x * player->movement.speed;
		return;
	}
	// NOTE: Fallback in case the entity does not have a rigidbody
	Position *pos = entity_get_position(game->world, player->entity);
	*pos = vec2_add(*pos, vec2_scale(input.movement, player->movement.speed));
}


static inline uint32_t dig_total_hardness(Game *game, Vec2i target) {
	static const int BRUSH_SIZE = 1;
	uint32_t sum = 0;
	for (int i = -BRUSH_SIZE; i <= BRUSH_SIZE; i++) {
	for (int j = -BRUSH_SIZE; j <= BRUSH_SIZE; j++) {
		int x = target.x + i;
		int y = target.y + j;
		Tile tile = map_get(game->map, x, y);
		if (tile_is_unbreakable(tile))
			continue;
		sum += tile_hardness(tile);
	}}
	return sum;
}

static inline void player_dig(Game *game, PlayerController* player, Vec2i target) {
	if (!input.digging) {
		return;
	}

	Work* work = &player->dig.work;
	if (!vec2i_eq(target, player->dig.target) || work_is_inactive(work)) {
		player->dig.target = target;
		uint32_t hardness = dig_total_hardness(game, target);
		if (hardness == 0) {
			return;
		}
		work_start(work, 10 * hardness);
		// printf(WORK_FMT"\n", WORK_ARG(*work));
	}

	WorkUnit progress = player->dig.speed;
	if (tile_is_unbreakable(player->dig.tile))
		progress = 0;
	if (!work_progress(work, progress))
		return;

	// printf(WORK_FMT"\n", WORK_ARG(*work));
	// MaterialComposition composition = get_tile_composition(player->dig.tile);
	// material_composition_merge_into(&player->bag, &composition);
	// material_composition_print(&player->bag);
	work_reset(work);
	static const int BRUSH_SIZE = 1;
	for (int i = -BRUSH_SIZE; i <= BRUSH_SIZE; i++) {
	for (int j = -BRUSH_SIZE; j <= BRUSH_SIZE; j++) {
		int x = target.x + i;
		int y = target.y + j;
		Tile tile = map_get(game->map, x, y);
		if (tile_is_unbreakable(tile))
			continue;
		map_set(game->map, x, y, TILE_AIR);
	}}
}


static inline Vec2i current_mouse_tile() {
	Vec2 world_mouse = vec2_add(camera_get_position(), input.mouse);
	Vec2i world_mouse_tile = world_to_tile_coord(world_mouse);
	return world_mouse_tile;
}



static inline void player_build(Game *game, Tile tile) {
	static const int BRUSH_SIZE = 3;
	Vec2i tile_position = current_mouse_tile();
	for (int i = 0; i < BRUSH_SIZE; i++)
	for (int j = 0; j < BRUSH_SIZE; j++) {
		int x = tile_position.x + i;
		int y = tile_position.y + j;
		if (!tile_is_unbreakable(map_get(game->map, x, y)))
			map_set(game->map, x, y, tile);
	}
}


// TODO: This is getting really bad, help
static inline void player_use(Game *game, PlayerController *player, PlayerToolSlot *slot) {
	if (slot->amount <= 0 && !slot->unlimited)
		return;
	if (!slot->unlimited && input.mouse_clicked)
		slot->amount--;

	// TODO: Naturally this will have to be made better
	Vec2i tile = current_mouse_tile();
	switch (slot->kind) {
	case PLAYER_TOOL_NONE: break;
	case PLAYER_TOOL_PICKAXE:
		// player_dig(player, tile);
		break;
	case PLAYER_TOOL_WOOD:
		player_build(game, TILE_WOOD);
		break;
	case PLAYER_TOOL_STONE:
		player_build(game, TILE_STONE);
		break;
	case PLAYER_TOOL_TORCH: {
		if (input.mouse_clicked)
			entity_create_torch(game->world, tile, 16);
	} break;
	case PLAYER_TOOL_CHEST:
		if (input.mouse_clicked)
		entity_create_furniture(game->world, tile, TEXTURE_FURNITURE_CHEST);
		break;
	case PLAYER_TOOL_FURNACE:
		if (input.mouse_clicked)
		entity_create_furniture(game->world, tile, TEXTURE_FURNITURE_FURNACE);
		break;
	case PLAYER_TOOL_WORKBENCH:
		if (input.mouse_clicked)
		entity_create_furniture(game->world, tile, TEXTURE_FURNITURE_WORKBENCH);
		break;
	case PLAYER_TOOL_ANVIL:
		if (input.mouse_clicked)
		entity_create_furniture(game->world, tile, TEXTURE_FURNITURE_ANVIL);
		break;
	case PLAYER_TOOL_LADDER:
		if (input.mouse_clicked)
		entity_create_furniture(game->world, tile, TEXTURE_FURNITURE_LADDER);
		break;
	}
}



void player_update(Game *game, PlayerController* player) {
	player_movement(game, player);
	player_dig(game, player, current_mouse_tile());
	if (input.randomize_player) {
		Entity temp = player->entity;
		player->entity = game->chest;
		game->chest = temp;
		// randomize_sprite(player);
	}
	Vec2 world_mouse = vec2_add(camera_get_position(), input.mouse);
	Vec2i world_mouse_tile = world_to_tile_coord(world_mouse);

	for (int i = 0; i < PLAYER_TOOLBAR_SIZE; i++) {
		if (input.slot[i + 1])
			player->current_tool = i;
	}
	int current_facing = player->is_facing_right ? 1 : -1;
	if (current_facing * input.movement.x < 0) {
		player->is_facing_right = input.movement.x > 0;
		SpriteId *sprite = entity_get_spriteid(game->world, player->entity);
		sprite_set_flip_x(*sprite, !player->is_facing_right);
	}
	
	if (input.mouse_clicked || input.mouse_held) {
		player_use(game, player, &player->tools[player->current_tool]);
	}
}


void player_render(Game *game, PlayerController* player) {
	static const Vec2 CENTER = {.x = GAME_WIDTH / 2.0, .y = GAME_HEIGHT / 2.0 };

	Position *pos = entity_get_position(game->world, player->entity);

	camera_set_position(vec2_sub(*pos, CENTER));
	// sprite_set_position(player->sprite, *position);
}

