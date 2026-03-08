#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>

#include "component_storage.h"
#include "components.h"
#include "config.h"


typedef struct {
	uint32_t size;
	bool alive[ENTITY_COUNT];
	uint16_t generation[ENTITY_COUNT];
	ComponentBitmap component_map[ENTITY_COUNT];

	uint16_t first_free;
	uint16_t next_free[ENTITY_COUNT];
} EntityContainer;


typedef struct world {
	EntityContainer entities;
	ComponentContainer components;
} World;


World world_create();
void world_print(World* world);

Entity entity_create(World* world);
void entity_destroy(World* world, Entity entity);
bool entity_is_alive(World* world, Entity entity);
bool entity_has_component(World *world, Entity entity, ComponentType component);
bool entity_has_components(World *world, Entity entity, ComponentBitmap components);


Entity query_begin(World* world, ComponentBitmap query);
void query_next(World* world, Entity* entity, ComponentBitmap query);
bool query_has_next(World* world, Entity* entity);


#define foreach_entity(world, entity, query)\
	for (\
		Entity entity = query_begin(world, query);\
		query_has_next(world, &entity);\
		query_next(world, &entity, query)\
	)

typedef ComponentBitmap Query;

#endif // ENTITY_H
