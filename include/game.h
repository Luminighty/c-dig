#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "linalg.h"
#include "lua_api.h"
#include "map.h"
#include "systems.h"
#include <stdint.h>

typedef struct game {
	uint64_t tick;
	World *world;
	Map *map;
	Systems systems;
	LuaState lua;
} Game;

extern Game game;


void game_init();
void game_destroy();

void game_draw();
void game_update();

Vec2 game_spawn_position();


#endif // GAME_H
