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


#endif // ENTITY_H
