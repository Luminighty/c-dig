#include "entity_prefabs.h"
#include "components.h"
#include "config.h"
#include "display.h"
#include "linalg.h"


Entity entity_create_fren(World *world, Vec2 position) {
	Entity entity = entity_create(world);

	SpriteId *sprite = entity_add_spriteid(world, entity);
	*sprite = sprite_create(TEXTURE_FREN, position, VEC2_ZERO);
	sprite_set_layer(*sprite, RENDER_LAYER_BACKGROUND);

	Position *pos = entity_add_position(world, entity);
	*pos = position;

	return entity;
}
