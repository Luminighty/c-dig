#include <raylib.h>
#include <stdio.h>
#include <unistd.h>

#include "client.h"
#include "cmdargs.h"
#include "config.h"
#include "game.h"
#include "server.h"


static void main_client() {

	InitWindow(WINDOW_DIMENSION.x, WINDOW_DIMENSION.y, WINDOW_TITLE);
	SetTargetFPS(60);

	game_init();
	game_client_init();
	
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
	// game_client_destroy();
}


static void main_server() {
	IS_HEADLESS = true;

	game_init();
	game_server_init();
	
	while (1) {
		game_update();

		fflush(stdout);
		fflush(stderr);

		usleep(50 * 1000); // 20 TPS
	}

	game_destroy();
	game_server_destroy();
}


int main(int argc, char *argv[]) {
	args_parse(argc, argv);
	args_debug();
	printf("%d\n", cmd_args.mode);
	switch (cmd_args.mode) {
		case RUNMODE_SOLO: main_client(); break;
		case RUNMODE_CLIENT: main_client(); break;
		case RUNMODE_SERVER: main_server(); break;
	}

	return 0;
}

