#ifndef CLIENT_ARGS_H
#define CLIENT_ARGS_H

#include <stdint.h>


typedef enum {
	RUNMODE_SINGLEPLAYER,
	RUNMODE_MULTIPLAYER,
} RunMode;


typedef struct {
	RunMode mode;
	const char* server_ipdarr;
	uint16_t port;
} CmdArgs;

extern CmdArgs cmd_args;

void args_parse(int argc, char *argv[]);
void args_debug();


#endif // ARGS_H


