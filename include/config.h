#ifndef CONFIG_H
#define CONFIG_H


#include "linalg.h"


#define WINDOW_TITLE "VOFI Dig: Slaves of Alderheim"


#define TILE_PIXEL_SIZE 4
#define ENTITY_COUNT 65535
#define LIGHT_LEVEL_FULL 32

#define MAP_WIDTH (64 * 4)
#define MAP_HEIGHT (64 * 4)


// NOTE: Game Viewport in pixels
#define GAME_HEIGHT (TILE_PIXEL_SIZE * (64 + 1))
#define GAME_WIDTH  (TILE_PIXEL_SIZE * (64 + 1))
#define GAME_SIZE   ((GAME_HEIGHT) * (GAME_WIDTH))

#define DEFAULT_PORT 61216
#define PING_DELAY 50

typedef enum {
	RENDER_LAYER_BACKGROUND,
	RENDER_LAYER_WALL,
	RENDER_LAYER_PLAYER,
	RENDER_LAYER_NORMAL,
	RENDER_LAYER_FOREGROUND,

	RENDER_LAYER_SIZE,
} RenderLayer;

////////////////
/// Settings ///
////////////////

extern int GAME_SCALE;
extern int UI_SCALE;

extern Vec2i WINDOW_DIMENSION;
// Returns the target viewport for the game
Rect2 game_viewport();


extern bool IS_HEADLESS;


#endif //  CONFIG_H
