#ifndef CLIENT_H
#define CLIENT_H


void game_client_init();
void *game_client_thread(void *arg);
void game_client_destroy();


#endif // CLIENT_H
