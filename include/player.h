#ifndef PLAYER_H
#define PLAYER_H


#include "display.h"
#include "math.h"
#include "tile.h"
#include "utils.h"
#include <raylib.h>
#include <stdbool.h>


typedef struct {
	Direction target;
	int progress;
	int speed;
	Tile tile;
} Dig;


typedef struct {
	int speed;
	int progress;
	int weight;
	int jump;
	bool on_ground;
} Movement;


typedef struct {
	Vec2 position;
	Movement movement;
	Dig dig;
	Glyph glyph;
} Player;


Player player_create(int x, int y);
void player_update(Player* player);
void player_render(Player* player);


#endif // PLAYER_H
