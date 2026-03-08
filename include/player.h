#ifndef PLAYER_H
#define PLAYER_H


#include "physics.h"
#include "display.h"
#include "entity_id.h"
#include "linalg.h"
#include "tile.h"
#include "utils.h"
#include <raylib.h>
#include <stdbool.h>


typedef struct {
	// Work work;
	// Direction target;
	int speed;
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
	PLAYER_SPRITE_SLOT_SIZE,
} PlayerSpriteSlot;

typedef enum {
	PLAYER_CURSOR_DESTROY,
	// PLAYER_CURSOR_STONE,
	// PLAYER_CURSOR_DIRT,
	PLAYER_CURSOR_WOOD,
	PLAYER_CURSOR_STONE,
	PLAYER_CURSOR_TORCH,
} PlayerCursorMode;


typedef struct {
	Entity entity;
	Movement movement;
	Dig dig;
	ColliderId collider;
	PlayerCursorMode cursor_mode;
	bool is_facing_right;
	SpriteId sprite;
	SpriteId sprites[PLAYER_SPRITE_SLOT_SIZE];
} Player;


struct world;
struct game;
Player player_create(struct world *world, int x, int y);
void player_update(struct game *game, Player* player);
void player_render(struct game *game, Player* player);


#endif // PLAYER_H
