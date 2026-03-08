#ifndef SYSTEMS_H
#define SYSTEMS_H


#include "darray.h"

#define SYSTEMS_SIZE 64


struct world;

typedef void (*SystemFn)(struct world *_world);

DATypedef(SystemArray, SystemFn);

typedef struct {
	SystemArray update;
	SystemArray init;
	SystemArray draw;
} Systems;


Systems systems_create();
Systems systems_destroy();

void system_on_update(SystemFn callback);

void systems_fire_init(Systems *systems, struct world *world);
void systems_fire_draw(Systems *systems, struct world *world);
void systems_fire_update(Systems *systems, struct world *world);

#endif // SYSTEMS_H
