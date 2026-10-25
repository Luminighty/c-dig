#include <raylib.h>
#include <stdio.h>
#include <unistd.h>

#include "client/client.h"
#include "client/args.h"
#include "client/client_app.h"
#include "client/render.h"
#include "config.h"
#include "game.h"
// #include "server.h"


static inline void main_client() {
	InitWindow(WINDOW_DIMENSION.x, WINDOW_DIMENSION.y, WINDOW_TITLE);
	SetTargetFPS(60);

	client_render_init();
	game_init();
	game_client_net_init();
	client_app_init();
	
	while (!WindowShouldClose()) {
		client_app_update();
		game_update();
		game_client_net_update();

		client_app_render();
		game_draw();
		BeginDrawing();
		ClearBackground(BLACK);
		client_render();
		EndDrawing();

		fflush(stdout);
		fflush(stderr);
	}

	game_destroy();
	client_render_destroy();
	client_app_destroy();
	// game_client_destroy();
}

int main(int argc, char *argv[]) {
	args_parse(argc, argv);
	args_debug();
	main_client();
	return 0;
}

