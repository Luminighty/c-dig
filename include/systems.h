#ifndef SYSTEMS_H
#define SYSTEMS_H


#include "darray.h"

#define SYSTEMS_SIZE 64


struct world;

typedef void (*SystemFn)(struct world *_world);

typedef DynamicArray(SystemFn) SystemArray;

typedef struct {
	SystemArray update;
	SystemArray init;
	SystemArray draw;
} Systems;


Systems systems_create();
Systems systems_destroy();

void systems_on_update(Systems *systems, SystemFn system);

void systems_fire_init(Systems *systems, struct world *world);
void systems_fire_draw(Systems *systems, struct world *world);
void systems_fire_update(Systems *systems, struct world *world);

#endif // SYSTEMS_H
