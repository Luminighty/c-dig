#include "entity.h"
#include "components.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


World world_create() {
	World w = {0};
	w.entities.size = 1;
	return w;
}


Entity entity_create(World* world) {
	uint16_t idx = world->entities.first_free;
	if (idx == 0) {
		assert(world->entities.size < ENTITY_COUNT);
		idx = world->entities.size++;
	} else {
		idx = world->entities.first_free;
		world->entities.first_free = world->entities.next_free[idx];
		world->entities.next_free[idx] = 0;
	}

	world->entities.alive[idx] = true;
	uint16_t gen = ++world->entities.generation[idx];
	return (Entity){.as = {.gen = gen, .index = idx}};
}


bool entity_is_alive(World* world, Entity entity) {
	return world->entities.alive[entity.as.index]
	    && world->entities.generation[entity.as.index] == entity.as.gen;
}


void entity_destroy(World* world, Entity entity) {
	if (!world->entities.alive[entity.as.index])
		return;
	world->entities.alive[entity.as.index] = false;
	world->entities.next_free[entity.as.index] = world->entities.first_free;
	world->entities.first_free = entity.as.index;
}


static inline bool is_query_matches(ComponentBitmap entity_components, ComponentBitmap query) {
	for (int i = 0; i < (COMPONENT_SIZE / COMPONENTBITMAP_SLOTSIZE) + 1; i++)
		if (entity_components.bytes[i] != (entity_components.bytes[i] | query.bytes[i]))
			return false;
	return true;
}


bool entity_has_component(World *world, Entity entity, ComponentType component) {
	if (!entity_is_alive(world, entity))
		return false;
	ComponentBitmap entity_components = world->entities.component_map[entity.as.index];
	return entity_components.bytes[component / COMPONENTBITMAP_SLOTSIZE] & COMPONENT_TO_SLOTBIT(component);
}


bool entity_has_components(World* world, Entity entity, ComponentBitmap components) {
	if (!entity_is_alive(world, entity))
		return false;
	ComponentBitmap entity_components = world->entities.component_map[entity.as.index];
	return is_query_matches(entity_components, components);
}


#define print_component(name, component_tag) if (entity_has_component(world, entity, component_tag)) printf(#name ", ");
void world_print(World *world) {
	foreach_entity(world, entity, ((ComponentBitmap){0})) {
		printf("Entity %d: %lu", entity.as.index, world->entities.component_map[entity.as.index].bytes[0]);
		printf("\n\t");

	#define DENSE(class, _, tag) print_component(class, tag)
	#define FLAG(class, _, tag) print_component(class, tag)
		COMPONENTS
	#undef FLAG
	#undef DENSE
		printf("\n");

	}
}
#undef print_component


Entity query_begin(World* world, ComponentBitmap query) {
	Entity entity = {0};
	query_next(world, &entity, query);
	return entity;
}


void query_next(World* world, Entity* entity, ComponentBitmap query) {
	uint16_t index = entity->as.index;
	while (1) {
		index++;
		if (index >= world->entities.size) {
			entity->as.index = index;
			entity->as.gen = 0;
			return;
		}
		if (!world->entities.alive[index])
			continue;
		ComponentBitmap entity_components = world->entities.component_map[index];
		if (!is_query_matches(entity_components, query))
			continue;
		entity->as.index = index;
		entity->as.gen = world->entities.generation[index];
		return;
	}
}


bool query_has_next(World* world, Entity* entity) {
	return entity->as.index < world->entities.size;
}

