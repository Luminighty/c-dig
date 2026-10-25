#include "net_entity.h"
#include "entity_id.h"
#include <stdlib.h>


Entity network_entity_find(NetworkEntities *entities, NetEntityId id) {
	for (size_t i = 0; i < entities->count; i++) {
		if (entities->items[i].id != id)
			continue;
		return entities->items[i].entity;
	}
	// da_push(*entities, ((NetworkEntityData){ .id = id, .entity = ENTITY_NONE }));
	return ENTITY_NONE;
}

