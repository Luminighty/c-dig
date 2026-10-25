#ifndef CLIENT_H
#define CLIENT_H

#include "components.h"

void game_client_net_init();
void game_client_net_update();
void game_client_net_destroy();

void game_client_net_add_entity(NetworkEntity *entity);

#endif // CLIENT_H
