#ifndef PLAYER_H
#define PLAYER_H


#include "collision.h"
#include "display.h"
#include "entity.h"
#include "material.h"
#include "linalg.h"
#include "tile.h"
#include "utils.h"
#include "work.h"
#include <raylib.h>
#include <stdbool.h>


typedef struct {
	Work work;
	Direction target;
	int speed;
	Tile tile;
} Dig;


typedef struct {
	float speed;
	int jump;
	bool on_ground;
	float gravity;
} Movement;


typedef enum {
	PLAYER_SPRITE_SLOT_BODY,
	PLAYER_SPRITE_SLOT_CLOTH,
	PLAYER_SPRITE_SLOT_BEARD,
	PLAYER_SPRITE_SLOT_HAT,
	PLAYER_SPRITE_SLOT_SIZE,
} PlayerSpriteSlot;


typedef struct {
	EntityId id;
	Vec2 position;
	Movement movement;
	Dig dig;
	MaterialComposition bag;
	ColliderId collider;
	SpriteId sprites[PLAYER_SPRITE_SLOT_SIZE];
} Player;


Player player_create(int x, int y);
void player_update(Player* player);
void player_render(Player* player);


#endif // PLAYER_H
