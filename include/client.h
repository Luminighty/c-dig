#ifndef CLIENT_H
#define CLIENT_H

#include "linalg.h"
#include "tile.h"


void game_client_init();
void *game_client_thread(void *arg);
void game_client_destroy();


void game_client_update_map(Tile tile, Rect2i rect);

#endif // CLIENT_H
