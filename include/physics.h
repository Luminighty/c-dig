#ifndef PHYSICS_H
#define PHYSICS_H

#include "entity_id.h"
#include "linalg.h"
#include "darray.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef uint32_t ColliderId;


typedef struct {
	Vec2 center;
	Vec2 extends;
} CollisionBox;


typedef struct {
	Entity parent;
	ColliderId id;
	CollisionBox box;
	bool alive;
	bool enabled;
	bool debug;
} Collider;
DATypedef(Colliders, Collider);


typedef struct {
	Colliders colliders;
} PhysicsServer;


typedef struct {
	Vec2 origin;
	Vec2 delta;
} Ray2;


typedef struct {
	Vec2 origin;
	Vec2 delta;
	Vec2 inv;
} Ray2Extended;


typedef struct {
	bool collided;
	bool is_inside_box;
	bool collided_with_tile;
	ColliderId other;
	CollisionBox other_box;
	float distance;
	Vec2 resolved_position;
	Vec2 collision_normal;
} ColliderMoveResult;


void physics_init();
void physics_destroy();
void physics_render();

ColliderId collider_create(Entity parent, Vec2 center, Vec2 extends);
Collider* collider_get(ColliderId id);

ColliderMoveResult collider_move(ColliderId id, Vec2 delta);

Ray2Extended ray2_create_ex(Vec2 origin, Vec2 delta);
Ray2 ray2_create(Vec2 origin, Vec2 delta);


#endif // PHYSICS_H
