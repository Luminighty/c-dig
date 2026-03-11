#include "systems.h"
#include "component_storage.h"
#include "components.h"
#include "darray.h"
#include "display.h"
#include "entity.h"
#include "entity_prefabs.h"
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

		float dx = random_frange(-0.01f, 0.01f);
		float dy = random_frange(-0.01f, 0.01f);

		if (position->x < 0)
			dx = 0.1f;
		if (position->x > 500)
			dx = -0.1f;
		if (position->y < 0)
			dy = 0.1f;
		if (position->y > 500)
			dy = -0.1f;

		position->x += dx;
		position->y += dy;
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

