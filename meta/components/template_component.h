#define entity_get_component(world, entity, COMPONENT) (\
	entity_is_alive((world), entity) \
	? &(world)->components.COMPONENT[entity.as.index] \
	: &(world)->components.COMPONENT[0] \
)

union entity;
struct world;

