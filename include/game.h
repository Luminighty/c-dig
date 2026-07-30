#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "entity_id.h"
#include "map.h"
#include "player.h"
#include "systems.h"
#include "ui.h"
#include <raylib.h>
#include <stdint.h>

typedef struct game {
	uint64_t tick;
	World *world;
	Map *map;
	PlayerController player;
	Systems systems;
	RenderTexture2D render_target;
	RenderTexture2D ui_target;
	UI ui;
	// NOTE: TEMP fields
	Entity other_player;
	Entity chest;
} Game;

extern Game game;


void game_init();
void game_destroy();

void game_draw();
void game_update();


#endif // GAME_H
