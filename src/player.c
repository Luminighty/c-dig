#include "player.h"
#include "config.h"
#include "display.h"
#include "game.h"
#include "input.h"
#include "map.h"
#include "material.h"
#include "math.h"
#include "palette.h"
#include "tile.h"
#include "utils.h"
#include "work.h"
#include <stdio.h>


Player player_create(int x, int y) {
	return (Player){
		.position = {.x = x, .y = y},
		.movement = {.speed = 100, .work_weight = 1000},
		.glyph = glyph(0x02, PAL_ORANGE, PAL_BLACK),
		.dig = { .speed = 100 }
	};
}


static inline void move(Player* player, int dx, int dy) {
	Vec2 new_pos = {.x = player->position.x + dx, .y = player->position.y + dy};
	Tile target_tile = map_get(game.map, new_pos.x, new_pos.y);
	if (tile_is_solid(target_tile))
		return;
	player->position = new_pos;
	work_reset(&player->dig.work);
}


static inline void player_gravity(Player* player) {
	static const int GRAVITY_TICK = 3;
	if (game.tick % GRAVITY_TICK != 0)
		return;
	if (player->movement.jump > 0)
		return;

	int dy = 1;
	Tile target_tile = map_get(game.map, player->position.x, player->position.y + dy);
	if (tile_is_solid(target_tile)) {
		player->movement.on_ground = true;
		return;
	}
	player->movement.on_ground = false;
	move(player, 0, dy);
}


static inline void player_jump(Player* player) {
	if (!input.jump)
		return;
	if (!player->movement.on_ground)
		return;
	player->movement.on_ground = false;
	player->movement.jump = 3;
}


static inline void player_movement(Player* player) {
	Vec2 delta = {
		.x = input.movement.x,
		.y = player->movement.jump > 0 ? -1 : 0,
	};
	bool is_stationary = vec2_is_zero(delta);

	bool is_done = work_full_update(
		&player->movement.work,
		!is_stationary,
		player->movement.work_weight,
		player->movement.speed
	);
	if (!is_done)
		return;

	if (delta.x)
		move(player, delta.x, 0);
	if (delta.y)
		move(player, 0, delta.y);
	if (player->movement.jump > 0)
		player->movement.jump--;
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
	player_gravity(player);
	player_jump(player);
}


void player_render(Player* player) {
	static const int x = SCREEN_WIDTH / 2;
	static const int y = SCREEN_HEIGHT / 2;
	display_putchar(x, y, player->glyph, 50);
}
