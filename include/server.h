#ifndef SERVER_H
#define SERVER_H


void game_server_init();
void *game_server_thread(void *arg);
void game_server_destroy();


#endif // SERVER_H
