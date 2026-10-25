#ifndef NET_ENTITY_H
#define NET_ENTITY_H


#include "darray.h"
#include "entity_id.h"
#include "netentity_id.h"

typedef struct {
	NetEntityId id;
	Entity entity;
	uint32_t owner;
} NetworkEntityData;

typedef DynamicArray(NetworkEntityData) NetworkEntities;


// TODO: Maybe we could speed this up to O(logn) or O(c)
Entity network_entity_find(NetworkEntities *entities, NetEntityId id);


#endif // NET_ENTITY_H
