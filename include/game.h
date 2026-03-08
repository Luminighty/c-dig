#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "map.h"
#include "player.h"
#include "systems.h"
#include <raylib.h>
#include <stdint.h>

typedef struct game {
	uint64_t tick;
	World world;
	Map *map;
	Player player;
	Systems systems;
	RenderTexture2D render_target;
} Game;

extern Game game;


void game_init();
void game_destroy();

void game_draw();
void game_update();


#endif // GAME_H
