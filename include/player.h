#ifndef PLAYER_H
#define PLAYER_H


#include "display.h"
#include "material.h"
#include "math.h"
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
	Work work;
	WorkUnit work_weight;
	int speed;
	int jump;
	bool on_ground;
} Movement;


typedef struct {
	Vec2 position;
	Glyph glyph;
	Movement movement;
	Dig dig;
	MaterialComposition bag;
} Player;


Player player_create(int x, int y);
void player_update(Player* player);
void player_render(Player* player);


#endif // PLAYER_H
