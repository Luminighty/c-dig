#ifndef PLAYER_H
#define PLAYER_H


#include "linalg.h"
#include "mineral.h"
#include "entity_id.h"
#include "tile.h"
#include "work.h"
#include <raylib.h>
#include <stdbool.h>


typedef struct {
	Work work;
	// Direction target;
	int speed;
	Vec2i target;
	Tile tile;
} Dig;


typedef struct {
	float speed;
	int jump;
} Movement;


typedef enum {
	PLAYER_SPRITE_SLOT_BODY,
	PLAYER_SPRITE_SLOT_CLOTH,
	PLAYER_SPRITE_SLOT_BEARD,
	PLAYER_SPRITE_SLOT_HAT,
	PLAYER_SPRITE_SLOT__SIZE,
} PlayerSpriteSlot;


typedef enum {
	PLAYER_TOOL_NONE,
	PLAYER_TOOL_PICKAXE,
	PLAYER_TOOL_WOOD,
	PLAYER_TOOL_STONE,
	PLAYER_TOOL_TORCH,
	PLAYER_TOOL_CHEST,
	PLAYER_TOOL_FURNACE,
	PLAYER_TOOL_WORKBENCH,
	PLAYER_TOOL_ANVIL,
	PLAYER_TOOL_LADDER,
} PlayerToolKind;


typedef struct {
	PlayerToolKind kind;
	bool unlimited;
	int amount;
} PlayerToolSlot;


typedef struct {
	uint32_t size;
	uint16_t amount[MINERAL__SIZE];
} PlayerBag;


#define PLAYER_TOOLBAR_SIZE 4


typedef struct {
	Entity entity;
	Movement movement;
	Dig dig;
	PlayerToolSlot tools[PLAYER_TOOLBAR_SIZE];
	PlayerBag bag;
	int current_tool;
	int tools_count;
	bool is_facing_right;
} PlayerController;


struct world;
struct game;
PlayerController player_create(struct world *world, Entity entity);
void player_update(struct game *game, PlayerController* player);
void player_render(struct game *game, PlayerController* player);
void player_render_interaction(struct game *game, PlayerController* player);

#endif // PLAYER_H
