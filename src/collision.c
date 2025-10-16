#include "collision.h"
#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "darray.h"
#include "debug.h"
#include "display.h"
#include "entity.h"
#include "game.h"
#include "input.h"
#include "linalg.h"
#include "map.h"
#include "tile.h"

#define EPSILON 0.001f

static PhysicsServer server = {0};

void physics_init() {
	// NOTE: Push the "NULL" collider
	da_push(server.colliders, (Collider){0});
}

void physics_destroy() {
	free(server.colliders.items);
	server.colliders.items = NULL;
}

#define for_colliders(id) for (ColliderId (id) = 1; (id) < server.colliders.count; (id)++)


ColliderId collider_create(EntityId parent, Vec2 center, Vec2 extends) {
	Collider collider = {
		.box = {.center = center, .extends = extends},
		.alive = true,
		.enabled = true,
		.parent = parent,
	};

	bool found_slot = false;
	ColliderId id;
	for_colliders(i) {
		if (server.colliders.items[i].alive)
			continue;
		found_slot = true;
		id = i;
		server.colliders.items[i] = collider;
		break;
	}
	if (!found_slot) {
		id = server.colliders.count;
		da_push(server.colliders, collider);
	}

	assert(server.colliders.items[id].alive);
	return id;
}

Collider* collider_get(ColliderId id) { return &server.colliders.items[id]; }


static inline Vec2 box_min(CollisionBox* box) {
	return (Vec2){
		.x = box->center.x - box->extends.x,
		.y = box->center.y - box->extends.y,
	};
}


static inline Vec2 box_max(CollisionBox* box) {
	return (Vec2){
		.x = box->center.x + box->extends.x,
		.y = box->center.y + box->extends.y,
	};
}


static inline bool ray_box_intersect(CollisionBox box, Ray2Extended ray, double* t_min, double* t_max) {
	Vec2 bmin = box_min(&box);
	Vec2 bmax = box_max(&box);

	double tx1 = (bmin.x - ray.origin.x) * ray.inv.x;
	double tx2 = (bmax.x - ray.origin.x) * ray.inv.x;

	*t_min = fmin(tx1, tx2);
	*t_max = fmax(tx1, tx2);

	double ty1 = (bmin.y - ray.origin.y) * ray.inv.y;
	double ty2 = (bmax.y - ray.origin.y) * ray.inv.y;

	*t_min = fmax(*t_min, fmin(ty1, ty2));
	*t_max = fmin(*t_max, fmax(ty1, ty2));

	return *t_max >= *t_min;
}


static inline Vec2 ray_box_normal(CollisionBox box, Ray2Extended ray, double t_hit) {
	Vec2 bmin = box_min(&box);
	Vec2 bmax = box_max(&box);

	double t_x1 = (bmin.x - ray.origin.x) * ray.inv.x;
	if (t_x1 == t_hit) return (Vec2){.x = -1, .y = 0};

	double t_x2 = (bmax.x - ray.origin.x) * ray.inv.x;
	if (t_x2 == t_hit) return (Vec2){.x = 1, .y = 0};

	double t_y1 = (bmin.y - ray.origin.y) * ray.inv.y;
	if (t_y1 == t_hit) return (Vec2){.x = 0, .y = -1};

	double t_y2 = (bmax.y - ray.origin.y) * ray.inv.y;
	if (t_y2 == t_hit) return (Vec2){.x = 0, .y = 1};

	// This cannot happen.
	assert(0);
}


static inline ColliderMoveResult run_pass(ColliderId id, Ray2Extended ray) {
	Collider* self = collider_get(id);
	ColliderMoveResult result = {
		.collided = false,
		.is_inside_box = false,
		.distance = 1.0f,
		.resolved_position = {
			.x = self->box.center.x + ray.delta.x,
			.y = self->box.center.y + ray.delta.y,
		}
	};
	for_colliders(other_id) {
		if (id == other_id)
			continue;
		Collider* other = collider_get(other_id);
		if (!other->alive || !other->enabled)
			continue;
		double t_min, t_max = 0;
		CollisionBox extended = other->box;
		extended.extends.x += self->box.extends.x;
		extended.extends.y += self->box.extends.y;
		if (!ray_box_intersect(extended, ray, &t_min, &t_max))
			continue;
		double t_hit = (t_min < 0.0) ? t_max : t_min;
		if (t_hit < 0.0 || t_hit > result.distance)
			continue;
		result.distance = t_hit;
		result.collided = true;
		result.other = other_id;
		result.is_inside_box = t_min < 0.0f;
		result.other_box = other->box;
	}

	Vec2 target = vec2_add(ray.origin, ray.delta);
	Vec2i from = world_to_tile_coord(vec2_sub(ray.origin, self->box.extends));
	Vec2i to = world_to_tile_coord(vec2_add(target, self->box.extends));
	Vec2i min = vec2i_min(from, to);
	Vec2i max = vec2i_max(from, to);
	Vec2 tile_offset = (Vec2){.x = TILE_PIXEL_SIZE / 2.0f, .y = TILE_PIXEL_SIZE / 2.0f};

	for (int y = min.y - 1; y <= max.y; y++) {
	for (int x = min.x - 1; x <= max.x; x++) {
		Tile tile = map_get(game.map, x, y);
		if (!tile_is_solid(tile))
			continue;
		double t_min = 0, t_max = 0;
		Vec2i tile_coord = {.x = x, .y = y};
		CollisionBox tile_box = (CollisionBox){
			.center = vec2_add(tile_to_world_coord(tile_coord), tile_offset),
			.extends = (Vec2){
				.x = TILE_PIXEL_SIZE / 2.f,
				.y = TILE_PIXEL_SIZE / 2.f,
			}
		};
		CollisionBox extended = tile_box;
		extended.extends.x += self->box.extends.x;
		extended.extends.y += self->box.extends.y;
		if (!ray_box_intersect(extended, ray, &t_min, &t_max))
			continue;
		double t_hit = (t_min < 0.0) ? t_max : t_min;
		if (t_hit < 0.0 || t_hit > result.distance)
			continue;
		result.distance = t_hit;
		result.collided = true;
		result.other = 0;
		result.is_inside_box = t_min < 0.0f;
		result.other_box = tile_box;
		result.collided_with_tile = true;
		assert(!isnan(t_hit));
	}}

	if (!result.collided)
		return result;
	float distance = result.distance + (result.is_inside_box ? EPSILON : -EPSILON) * 2.f;
	result.resolved_position.x = ray.origin.x + ray.delta.x * distance;
	result.resolved_position.y = ray.origin.y + ray.delta.y * distance;
	return result;
}


static inline Vec2 vector2_reject(Vec2 a, Vec2 b) {
	float dot_b = vec2_dot(b, b);
	assert(dot_b > EPSILON);
	float scale = vec2_dot(a, b) / dot_b;
	Vec2 proj_a = vec2_scale(b, scale);
	return vec2_sub(a, proj_a);
}


// #define DEBUG_ONLY
ColliderMoveResult collider_move(ColliderId id, Vec2 delta) {
	Collider* self = collider_get(id);
	if (vec2_lengthsqr(delta) < EPSILON) {
		// NOTE: We assume that the body did not move
		return (ColliderMoveResult){
			.collided = false,
			.resolved_position = self->box.center,
			.distance = 0.f,
			.is_inside_box = false,
		};
	}
	Ray2Extended ray = ray2_create_ex(self->box.center, delta);
	ColliderMoveResult result = run_pass(id, ray);
	if (!result.collided) {
		self->box.center = result.resolved_position;
		return result;
	}

	#ifdef DEBUG_ONLY
	CollisionBox temp_box = collider_get(id)->box;
	#endif
	
	// NOTE: Calculate slide vector
	CollisionBox extended = result.other_box;
	extended.extends.x += self->box.extends.x;
	extended.extends.y += self->box.extends.y;
	Vec2 normal = ray_box_normal(extended, ray, result.distance);
	delta = vec2_scale(delta, 1.0f - result.distance);
	delta = vector2_reject(delta, normal);
	
	self->box.center = result.resolved_position;
	ray = ray2_create_ex(self->box.center, delta);
	ColliderMoveResult slide_result = run_pass(id, ray);

	if (slide_result.collided)
		result = slide_result;
	
	self->box.center = slide_result.resolved_position;

	#ifdef DEBUG_ONLY
	collider_get(id)->box = temp_box;
	#endif
	return result;
}


inline Ray2Extended ray2_create_ex(Vec2 origin, Vec2 delta) {
	return (Ray2Extended){
		.origin = origin,
		.delta = delta,
		.inv = {
			.x = 1.f / delta.x,
			.y = 1.f / delta.y,
		}
	};
}


inline Ray2 ray2_create(Vec2 origin, Vec2 delta) {
	return (Ray2){
		.origin = origin,
		.delta = delta,
	};
}

void physics_render() {
	for_colliders(id) {
		Collider* self = collider_get(id);
		if (!self->alive || !self->debug)
			continue;
		draw_box(
			self->box.center.x - self->box.extends.x,
			self->box.center.y - self->box.extends.y,
			self->box.extends.x * 2,
			self->box.extends.y * 2,
			self->enabled ? GREEN : GRAY
		);
	}

	#ifdef DEBUG_MAP
	Vec2i min = {.x = 0, .y = 0};
	Vec2i max = {.x = MAP_WIDTH, .y = MAP_HEIGHT};
	Vec2 tile_offset = (Vec2){.x = TILE_PIXEL_SIZE / 2.0f, .y = TILE_PIXEL_SIZE / 2.0f};

	Vec2 world_mouse = vec2_add(camera_get_position(), input.mouse);
	Vec2i world_mouse_tile = world_to_tile_coord(world_mouse);

	for (int y = min.y; y < max.y; y++) {
	for (int x = min.x; x < max.x; x++) {
		Tile tile = map_get(game.map, x, y);
		if (!tile_is_solid(tile))
			continue;
		Vec2i tile_coord = {.x = x, .y = y};
		CollisionBox tile_box = (CollisionBox){
			.center = vec2_add(tile_to_world_coord(tile_coord), tile_offset),
			.extends = (Vec2){
				.x = TILE_PIXEL_SIZE / 2.f,
				.y = TILE_PIXEL_SIZE / 2.f,
			}
		};
		draw_box(
			tile_box.center.x - tile_box.extends.x,
			tile_box.center.y - tile_box.extends.y,
			tile_box.extends.x * 2,
			tile_box.extends.y * 2,
			world_mouse_tile.x == x && world_mouse_tile.y == y ? GREEN : GRAY
		);
	}}
	if (input.clicked) {
		vec2i_print(world_mouse_tile);
		printf("\n");
	}
	#endif // DEBUG_MAP
}
