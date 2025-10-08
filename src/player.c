#include "player.h"
#include "config.h"
#include "display.h"
#include "game.h"
#include "input.h"
#include "map.h"
#include "math.h"
#include "palette.h"
#include "tile.h"
#include "utils.h"
#include <stdio.h>


Player player_create(int x, int y) {
	return (Player){
		.position = {.x = x, .y = y},
		.movement = {.weight = 30, .speed = 10},
		.glyph = glyph(0x01, PAL_WHITE, PAL_BLACK),
		.dig = { .speed = 10 }
	};
}


static inline void move(Player* player, Vec2 new_pos) {
	Tile target_tile = map_get(game.map, new_pos.x, new_pos.y);
	if (tile_is_solid(target_tile))
		return;
	player->position = new_pos;
	player->movement.progress = player->movement.weight;
	player->dig.target = 0; // NOTE: Reset digging when moving
}

static inline void player_gravity(Player* player) {
	static const int GRAVITY_TICK = 3;
	if (game.tick % GRAVITY_TICK != 0)
		return;
	if (player->movement.jump > 0)
		return;

	Vec2 new_pos = player->position;
	new_pos.y += 1;
	Tile target_tile = map_get(game.map, new_pos.x, new_pos.y);
	if (tile_is_solid(target_tile)) {
		player->movement.on_ground = true;
		return;
	}
	player->movement.on_ground = false;
	move(player, new_pos);
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
	Vec2 delta = {0};
	delta.x += input.movement.x;
	if (player->movement.jump > 0)
		delta.y -= 1;
	Vec2 new_pos = vec2_add(player->position, delta);
	bool is_stationary = vec2_is_zero(delta);
	if (is_stationary)
		player->movement.progress = player->movement.weight;

	if (player->movement.progress > player->movement.speed) {
		player->movement.progress -= player->movement.speed;
		return;
	}
	if (player->movement.jump > 0)
		player->movement.jump--;
	move(player, new_pos);
}


static inline void player_dig(Player* player) {
	if (!input.digging) {
		player->dig.target = 0;
		return;
	}
	Direction dir = vec2_into_dir(input.movement);
	if (dir != player->dig.target) {
		player->dig.target = dir;
		player->dig.progress = 100;

		Vec2 tile_pos = vec2_add(player->position, dir_into_vec2(dir));
		player->dig.tile = map_get(game.map, tile_pos.x, tile_pos.y);
	}
	if (tile_is_unbreakable(player->dig.tile))
		return;

	printf("Breaking %d\n", player->dig.progress);
	if (player->dig.progress < player->dig.speed) {
		player->dig.progress = 0;
		Vec2 tile_pos = vec2_add(player->position, dir_into_vec2(dir));
		map_set(game.map, tile_pos.x, tile_pos.y, TILE_AIR);
	} else {
		player->dig.progress -= player->dig.speed;
	}
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
