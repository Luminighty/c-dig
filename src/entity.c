#include "entity.h"


EntityId last_entity = 1;

EntityId entity_create() {
	return last_entity++;
}
