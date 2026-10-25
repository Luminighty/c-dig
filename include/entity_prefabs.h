#ifndef ENTITY_PREFAB_H
#define ENTITY_PREFAB_H

#include "entity.h"
#include "linalg.h"
#include "texture_registry.h"

typedef struct {
	Vec2 position;
	TextureId body;
	TextureId cloth;
	TextureId beard;
	TextureId hat;
} EntityPrefabPropsMiner;
EntityPrefabPropsMiner entity_miner_get_props(World *world, Entity entity);
Entity entity_create_miner(World *world, EntityPrefabPropsMiner props);

Entity entity_create_fren(World *world, Vec2 position);
Entity entity_create_torch(World *world, Vec2i tile, int intensity);
Entity entity_create_furniture(World *world, Vec2i tile, TextureId texture);

#endif // ENTITY_PREFAB_H
