#include "cmdargs.h"
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


CmdArgs cmd_args = {
	.mode = RUNMODE_CLIENT,
	.server_ipdarr = "127.0.0.1",
	.port = DEFAULT_PORT,
};


typedef struct {
	char **args;
	int count;
	int index;
} CmdArgIter;


static inline bool has_next(CmdArgIter *iter) {
	return iter->index < iter->count;
}

static inline char *next(CmdArgIter *iter) {
	if (iter->index >= iter->count)
		return NULL;
	return iter->args[iter->index++];
}

static inline char *peek(CmdArgIter *iter) {
	if (iter->index >= iter->count)
		return NULL;
	return iter->args[iter->index];
}

static inline bool match(CmdArgIter *iter, const char* arg) {
	if (iter->index >= iter->count)
		return false;
	if (strcmp(peek(iter), arg) == 0) {
		next(iter);
		return true;
	}
	return false;
}

#define assert_arg(cond, err_msg) do {                   \
	if (!(cond)) { fprintf(stderr, err_msg); exit(1); } \
} while (0)

// NOTE: This is an incredibly dumb, not designed args parser, 
// 	since I just wanted to get it out really quick
// 	Once this requires expansion, please look into how
// 	I am parsing cmdargs in ib/ect
void args_parse(int argc, char *argv[]) {
	CmdArgIter iter = {.args = argv, .count = argc, .index = 1};

	if (match(&iter, "server"))
		cmd_args.mode = RUNMODE_SERVER;
	if (match(&iter, "solo"))
		cmd_args.mode = RUNMODE_SOLO;
	if (match(&iter, "client"))
		cmd_args.mode = RUNMODE_CLIENT;

	while (has_next(&iter)) {
		char *curr = next(&iter);
		if (strcmp(curr, "-ip") == 0) {
			char * server_str = next(&iter);
			assert_arg(server_str != NULL, "IP address is missing!\n");
			// TODO: Validate server ip addr string

			cmd_args.server_ipdarr = server_str;
			continue;
		}

		if (strcmp(curr, "-port") == 0) {
			char *port_str = next(&iter);
			assert_arg(port_str != NULL, "Port is missing!\n");
			cmd_args.port = atoi(port_str);

			bool is_actually_zero = strlen(port_str) == 1 && port_str[0] == '0';
			assert_arg(cmd_args.port != 0 || is_actually_zero, "Port was not a number!\n");
			continue;
		}
	}
}

static const char* RUNMODE_LABEL[] = {
	[RUNMODE_CLIENT] = "CLIENT",
	[RUNMODE_SERVER] = "SERVER",
};

void args_debug() {
	printf("CMD Args:\n");
	printf("  mode: %s\n", RUNMODE_LABEL[cmd_args.mode]);
	printf("    ip: %s\n", cmd_args.server_ipdarr);
	printf("  port: %d\n", cmd_args.port);
	

}

