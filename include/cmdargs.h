#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>


typedef enum {
	RUNMODE_CLIENT,
	RUNMODE_SERVER,
} RunMode;


typedef struct {
	RunMode mode;
	const char* server_ipdarr;
	uint16_t port;
} CmdArgs;

extern CmdArgs cmd_args;

void args_parse(int argc, char *argv[]);


#endif // ARGS_H
