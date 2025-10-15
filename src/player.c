#include "player.h"
#include "assets_registry.h"
#include "collision.h"
#include "config.h"
#include "display.h"
#include "entity.h"
#include "game.h"
#include "input.h"
#include "map.h"
#include "material.h"
#include "linalg.h"
#include "random.h"
#include "tile.h"
#include "utils.h"
#include "work.h"
#include "debug.h"

#include <stdio.h>




Player player_create(int x, int y) {
	EntityId id = entity_create();
	Vec2 position = {.x = x, .y = y};
	ColliderId collider = collider_create(id, position, (Vec2){.x = 4, .y = 4});
	collider_get(collider)->debug = false;

	Vec2 sprite_offset = (Vec2){.x = -8, .y = -13};

	Player player = {
		.id = id,
		.position = {.x = x, .y = y},
		.movement = {.speed = 1.f, .gravity = 1.f},
		.dig = { .speed = 100 },
		.collider = collider,
	};

	TextureId body = random_range(TEXTURE_MINER_BODY_00, TEXTURE_MINER_BODY_05 + 1);
	TextureId cloth = random_range(TEXTURE_MINER_CLOTH_00, TEXTURE_MINER_CLOTH_05 + 1);
	TextureId beard = random_range(TEXTURE_MINER_BEARD_00, TEXTURE_MINER_BEARD_05 + 1);
	TextureId hat = random_range(TEXTURE_MINER_HAT_00, TEXTURE_MINER_HAT_05 + 1);
	player.sprites[PLAYER_SPRITE_SLOT_BODY] = sprite_create(body, position, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_CLOTH] = sprite_create(cloth, position, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_BEARD] = sprite_create(beard, position, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_HAT] = sprite_create(hat, position, sprite_offset);
	return player;
}

static void randomize_sprite(Player* player) {
	TextureId body = random_range(TEXTURE_MINER_BODY_00, TEXTURE_MINER_BODY_05 + 1);
	TextureId cloth = random_range(TEXTURE_MINER_CLOTH_00, TEXTURE_MINER_CLOTH_05 + 1);
	TextureId beard = random_range(TEXTURE_MINER_BEARD_00, TEXTURE_MINER_BEARD_05 + 1);
	TextureId hat = random_range(TEXTURE_MINER_HAT_00, TEXTURE_MINER_HAT_05 + 1);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_BODY], body);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_CLOTH], cloth);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_BEARD], beard);
	sprite_set_texture(player->sprites[PLAYER_SPRITE_SLOT_HAT], hat);
}


static inline void player_movement(Player* player) {
	Vec2 delta = {
		.x = input.movement.x * player->movement.speed,
		.y = player->movement.gravity
	};
	ColliderMoveResult result = collider_move(player->collider, delta);
	player->position = result.resolved_position;
}


static inline void player_dig(Player* player) {
	if (!input.digging) {
		player->dig.target = 0;
		return;
	}
	Direction dir = vec2_into_dir(input.movement);
	if (!dir)
		return;

	Work* work = &player->dig.work;
	if (dir != player->dig.target || work_is_inactive(work)) {
		Vec2 tile_pos = vec2_add(player->position, dir_into_vec2(dir));
		Tile tile = map_get(game.map, tile_pos.x, tile_pos.y);
		player->dig.tile = tile;
		player->dig.target = dir;
		work_start(work, 100 + 200 * tile_get_hardness(tile));
	}

	WorkUnit progress = player->dig.speed;
	if (tile_is_unbreakable(player->dig.tile))
		progress = 0;
	if (!work_progress(work, progress))
		return;
	printf("DONE\n");
	work_print(work); printf("\n");
	MaterialComposition composition = get_tile_composition(player->dig.tile);
	material_composition_merge_into(&player->bag, &composition);
	material_composition_print(&player->bag);
	work_reset(work);
	Vec2 tile_pos = vec2_add(player->position, dir_into_vec2(dir));
	map_set(game.map, tile_pos.x, tile_pos.y, TILE_AIR);
}


void player_update(Player* player) {
	player_movement(player);
	player_dig(player);
	if (input.randomize_player)
		randomize_sprite(player);
}


void player_render(Player* player) {
	static const int x = SCREEN_WIDTH * TILE_PIXEL_SIZE / 2;
	static const int y = SCREEN_HEIGHT * TILE_PIXEL_SIZE / 2;
	camera_set_position((Vec2){.x = player->position.x - x, .y = player->position.y - y});

	for (int slot = 0; slot < PLAYER_SPRITE_SLOT_SIZE; slot++)
		sprite_set_position(player->sprites[slot], player->position);
}
