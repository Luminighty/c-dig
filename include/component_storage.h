#ifndef COMPONENT_STORAGE_H
#define COMPONENT_STORAGE_H


#include <stdint.h>
#include "components.h"
#include "config.h"

typedef struct {
#define DENSE(type, name, _) type name [ENTITY_COUNT];
// NOTE: Flags don't contain data, so we skip them
#define FLAG(type, name, _)
	COMPONENTS
#undef FLAG
#undef DENSE
} ComponentContainer;

#define componentbitmap_create(...) _componentbitmap_create(__VA_ARGS__, COMPONENT_SIZE)
ComponentBitmap _componentbitmap_create(ComponentType c, ...);

// NOTE: I may need to generate this, but it's unused atm
#define is_query_empty(query) !(query .bytes[0])
#define StaticComponentBitmap(query) static ComponentBitmap query = {0}; if (is_query_empty(query)) query

#endif // COMPONENT_STORAGE_H
