#include "systems.h"
#include "component_storage.h"
#include "components.h"
#include "darray.h"
#include "display.h"
#include "entity.h"
#include "entity_prefabs.h"
#include "game.h"
#include "linalg.h"
#include "map.h"
#include "physics.h"
#include "random.h"
#include "stdlib.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>


static const float MAX_GRAVITY = 16.0f;

static void init_system(World *world) {
	Vec2 fren_spawn = tile_to_world_coord(((Vec2i){.x = 15, .y = MAP_HEIGHT - 40}));

	entity_create_fren(world, fren_spawn);
}

static void random_walk_system(World *world) {
	Query query = componentbitmap_create(COMPONENT_POSITION, COMPONENT_RANDOMWALK);

	foreach_entity(world, entity, query) {
		Position *position = entity_get_position(world, entity);
		RandomWalk *randomwalk = entity_get_randomwalk(world, entity);
		Vec2 delta = vec2_sub(randomwalk->target, *position);
		if (fabsf(delta.x) > 1.0f || fabsf(delta.y) > 1.0f) {
			Vec2 move = vec2_scale(vec2_normalize(delta), 0.2f);
			*position = vec2_add(*position, move);
			// printf("%f,%f\n", position->x, position->y);
			continue;
		}

		// Vec2 target_delta = {
		// 	.x = random_frange(-30.0, 30.0),
		// 	.y = random_frange(-30.0, 30.0)
		// };
		// Vec2 new_pos = vec2_add(*position, target_delta);
		Position *player_pos = entity_get_position(world, game.player.entity);
		Vec2 new_pos = vec2_add(*player_pos, (Vec2){ .x = -50.0, .y = -30.0, });

		// if (new_pos.x < 0)
		// 	new_pos.x = 30.0f;
		// if (new_pos.x > 500)
		// 	new_pos.x = -30.0f;
		// if (new_pos.y < 0)
		// 	new_pos.y = 30.0f;
		// if (new_pos.y > 500)
		// 	new_pos.y = -30.0f;

		randomwalk->target = new_pos;
	}
}

static void physics_system(World *world) {
	Query query = componentbitmap_create(COMPONENT_RIGIDBODY);

	foreach_entity(world, entity, query) {
		Rigidbody *rb = entity_get_rigidbody(world, entity);

		bool has_position = entity_has_component(world, entity, COMPONENT_POSITION);
		Position *position = entity_get_position(world, entity);
		
		if (has_position) {
			Collider *col = collider_get(rb->id);
			col->box.center = *position;
		}

		rb->velocity.y = fminf(rb->velocity.y + rb->gravity, MAX_GRAVITY);
		Vec2 delta = rb->velocity;
		ColliderMoveResult result = collider_move(rb->id, delta);

		if (has_position)
			*position = result.resolved_position;

		rb->on_ground = result.collision_normal.y < -0.5f;
		if (fabs(result.collision_normal.y) > 0.5f)
			rb->velocity.y = 0.f;
	}
}

static void sprite_system(World *world) {
	Query query = componentbitmap_create(
		COMPONENT_SPRITEID, COMPONENT_POSITION
	);

	foreach_entity(world, entity, query) {
		SpriteId *sprite = entity_get_spriteid(world, entity);
		Position *position = entity_get_position(world, entity);
		sprite_set_position(*sprite, *position);
	}
}

static void debug_system(World *world) {
	printf("LOG\n");
}


Systems systems_create() {
	Systems systems = {0};
	da_push(systems.init, init_system);
	da_push(systems.update, physics_system);
	da_push(systems.update, random_walk_system);
	da_push(systems.draw, sprite_system);
	// da_push(systems.update, system_log);
	return systems;
}


void systems_fire_init(Systems *systems, World *world) {
	for (size_t i = 0; i < systems->init.count; i++) {
		(systems->init.items[i])(world);
	}
}


void systems_fire_update(Systems *systems, World *world) {
	for (size_t i = 0; i < systems->update.count; i++) {
		(systems->update.items[i])(world);
	}
}

void systems_fire_draw(Systems *systems, World *world) {
	for (size_t i = 0; i < systems->draw.count; i++) {
		(systems->draw.items[i])(world);
	}
}

