#include "player.h"
#include "assets_registry.h"
#include "components.h"
#include "physics.h"
#include "config.h"
#include "display.h"
#include "entity.h"
#include "game.h"
#include "input.h"
#include "map.h"
// #include "material.h"
#include "linalg.h"
#include "random.h"
#include "tile.h"
// #include "utils.h"
// #include "work.h"
#include "debug.h"

#include <math.h>
#include <stdio.h>


Player player_create(World *world, int x, int y) {
	Entity entity = entity_create(world);

	Position *position = entity_add_position(world, entity);
	*position = tile_to_world_coord((Vec2i){.x=x, .y=y});

	Rigidbody *rb = entity_add_rigidbody(world, entity);
	rb->id = collider_create(entity, *position, (Vec2){.x = 3, .y = 5});
	rb->gravity = 0.1f;

	Vec2 sprite_offset = (Vec2){.x = -8, .y = -11};


	Player player = {
		.entity = entity,
		.movement = {.speed = 1.5f},
		.dig = { .speed = 100 },
		.collider = rb->id,
	};

	TextureId body = TEXTURE_MINER_BODY_00;
	TextureId cloth = TEXTURE_MINER_CLOTH_01;
	TextureId beard = TEXTURE_MINER_BEARD_01;
	TextureId hat = TEXTURE_MINER_HAT_03;

	SpriteId* sprite = entity_add_spriteid(world, entity);
	*sprite = sprite_create(body, *position, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_BODY] = *sprite;
	player.sprites[PLAYER_SPRITE_SLOT_CLOTH] = sprite_create(cloth, VEC2_ZERO, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_BEARD] = sprite_create(beard, VEC2_ZERO, sprite_offset);
	player.sprites[PLAYER_SPRITE_SLOT_HAT] = sprite_create(hat, VEC2_ZERO, sprite_offset);
	sprite_set_parent(player.sprites[PLAYER_SPRITE_SLOT_CLOTH], *sprite);
	sprite_set_parent(player.sprites[PLAYER_SPRITE_SLOT_BEARD], *sprite);
	sprite_set_parent(player.sprites[PLAYER_SPRITE_SLOT_HAT], *sprite);

	world_print(world);
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
	for (int j = 0; j < DIG_SIZE; j++)
		map_set(game->map, tile_position.x + i, tile_position.y + j, tile);
}


static char *CURSOR_LABELS[] = {
	[PLAYER_CURSOR_DESTROY] = "DESTROY",
	[PLAYER_CURSOR_WOOD] = "WOOD",
};

void player_update(Game *game, Player* player) {
	player_movement(game, player);
	// player_dig(player);
	if (input.randomize_player)
		randomize_sprite(player);
	Vec2 world_mouse = vec2_add(camera_get_position(), input.mouse);
	Vec2i world_mouse_tile = world_to_tile_coord(world_mouse);

	if (input.slot[1])
		player->cursor_mode = PLAYER_CURSOR_DESTROY;
	if (input.slot[2])
		player->cursor_mode = PLAYER_CURSOR_WOOD;
	debug_log("Cursor: %s", CURSOR_LABELS[player->cursor_mode]);

	
	if (input.clicked) {
		switch (player->cursor_mode) {
		case PLAYER_CURSOR_WOOD: player_build(game, TILE_WOOD); break;

		case PLAYER_CURSOR_DESTROY:
		default: player_build(game, TILE_AIR); break;
		}
	}
}


void player_render(Game *game, Player* player) {
	static const int x = SCREEN_WIDTH * TILE_PIXEL_SIZE / 2;
	static const int y = SCREEN_HEIGHT * TILE_PIXEL_SIZE / 2;
	Position *position = entity_get_position(&game->world, player->entity);

	camera_set_position((Vec2){.x = position->x - x, .y = position->y - y});
	// sprite_set_position(player->sprite, *position);
}

