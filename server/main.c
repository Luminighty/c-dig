#include <raylib.h>
#include <stdio.h>
#include <unistd.h>

#include "server/args.h"
#include "config.h"
#include "game.h"
#include "server/server.h"


static void main_server() {
	IS_HEADLESS = true;

	game_init();
	game_server_init();
	
	while (1) {
		game_update();
		game_server_update();

		fflush(stdout);
		fflush(stderr);

		usleep(NET_DELAY_MS * 1000); // 20 TPS
	}

	game_destroy();
	game_server_destroy();
}


int main(int argc, char *argv[]) {
	args_parse(argc, argv);
	args_debug();
	main_server();
	return 0;
}

