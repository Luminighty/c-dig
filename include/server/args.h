#ifndef SERVER_ARGS_H
#define SERVER_ARGS_H

#include <stdint.h>


typedef struct {
	uint16_t port;
} CmdArgs;

extern CmdArgs cmd_args;

void args_parse(int argc, char *argv[]);
void args_debug();


#endif // SERVER_ARGS_H
