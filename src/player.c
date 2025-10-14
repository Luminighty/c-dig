#include "player.h"
#include "collision.h"
#include "config.h"
#include "display.h"
#include "entity.h"
#include "game.h"
#include "input.h"
#include "map.h"
#include "material.h"
#include "linalg.h"
#include "palette.h"
#include "tile.h"
#include "utils.h"
#include "work.h"
#include <stdio.h>


Player player_create(int x, int y) {
	EntityId id = entity_create();
	ColliderId collider = collider_create(id, (Vec2){.x = x, .y = y}, (Vec2){.x = 2, .y = 2});
	return (Player){
		.id = id,
		.position = {.x = x, .y = y},
		.movement = {.speed = 0.1f, .gravity = 0.1f},
		.glyph = glyph(0x02, PAL_ORANGE, PAL_BLACK),
		.dig = { .speed = 100 },
		.collider = collider,
	};
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
}


void player_render(Player* player) {
	static const int x = SCREEN_WIDTH / 2;
	static const int y = SCREEN_HEIGHT / 2;
	display_putchar(x, y, player->glyph, 50);
}
