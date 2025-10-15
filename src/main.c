#include "config.h"
#include "display.h"
#include "game.h"
#include <stdio.h>
#include <raylib.h>

int main() {
	InitWindow(TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);
	
	game_init();
	
	while (!WindowShouldClose()) {
		game_update();

		BeginDrawing();
		ClearBackground(BLACK);
		game_draw();
		EndDrawing();

		fflush(stdout);
		fflush(stderr);
	}
	game_destroy();

	return 0;
}
