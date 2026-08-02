#include "config.h"
#include "linalg.h"


static const int DEFAULT_GAME_SCALE = 2;
static const int DEFAULT_UI_SCALE = 2;

bool IS_HEADLESS = false;

int GAME_SCALE = DEFAULT_GAME_SCALE;
int UI_SCALE = DEFAULT_UI_SCALE;


Vec2i WINDOW_DIMENSION = {
	.x = DEFAULT_GAME_SCALE * GAME_WIDTH + 200,
	.y = DEFAULT_GAME_SCALE * GAME_HEIGHT + 200,
};


Rect2 game_viewport() {
	Rect2 viewport = {
		.w = GAME_WIDTH * GAME_SCALE,
		.h = GAME_HEIGHT * GAME_SCALE
	};
	viewport.pos = vec2_sub(
		(Vec2){.x = WINDOW_DIMENSION.x, .y = WINDOW_DIMENSION.y}, 
		viewport.size
	);
	viewport.pos = vec2_div(viewport.pos, 2);
	return viewport;
}

