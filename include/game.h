#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <raylib.h>

#include "map.h"
#include "player.h"

typedef struct {
	Map* map;
	Player player;
	uint64_t tick;
} Game;

void game_init();
void game_draw();
void game_update();
void game_destroy();

extern Game game;

#endif // GAME_H
