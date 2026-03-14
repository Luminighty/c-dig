#ifndef ENTITY_PREFAB_H
#define ENTITY_PREFAB_H

#include "entity.h"
#include "linalg.h"


Entity entity_create_fren(World *world, Vec2 position);
Entity entity_create_torch(World *world, Vec2i tile, int intensity);
Entity entity_create_furniture(World *world, Vec2i tile, TextureId texture);

#endif // ENTITY_PREFAB_H
