#ifndef ENTITY_ID_H
#define ENTITY_ID_H

#include <stdint.h>


typedef union entity {
	uint32_t id;
	struct {
		uint16_t gen;
		uint16_t index;
	} as;
} Entity;

#define EntityFMT "Entity(#%d, gen%d)"
#define EntityARG(entity) ((entity).as.index), ((entity).as.gen)

static const Entity ENTITY_NONE = {0};


#endif // ENTITY_H
