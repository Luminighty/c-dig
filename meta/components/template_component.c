#define ENTITY_SET_TAG(world, entity, component)\
	world->entities.component_map[entity.as.index].bytes[component / COMPONENTBITMAP_SLOTSIZE] |= COMPONENT_TO_SLOTBIT(component)
#define ENTITY_UNSET_TAG(world, entity, component)\
	world->entities.component_map[entity.as.index].bytes[component / COMPONENTBITMAP_SLOTSIZE] &= ~COMPONENT_TO_SLOTBIT(component)

#define ENTITY_ADD_DENSE(world, entity, container, data, component_tag)\
	if (!entity_is_alive(world, entity))\
		return &world->components.container[0];\
	ENTITY_SET_TAG(world, entity, component_tag);\
	world->components.container[entity.as.index] = data;\
	return &world->components.container[entity.as.index]

#define ENTITY_ADD_FLAG(world, entity, container, data, component_tag)\
	if (!entity_is_alive(world, entity))\
		return;\
	ENTITY_SET_TAG(world, entity, component_tag);

#define ENTITY_REMOVE(world, entity, component_tag)\
	if (!entity_is_alive(world, entity))\
		return;\
	ENTITY_UNSET_TAG(world, entity, component_tag);

