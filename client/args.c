#include "client/args.h"

#include "cmd_parser.h"
#include "config.h"
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


CmdArgs cmd_args = {
	.mode = RUNMODE_SINGLEPLAYER,
	.server_ipdarr = "127.0.0.1",
	.port = DEFAULT_PORT,
};


#define match cmd_args_match
#define next cmd_args_next

#define check_arg(cond, err_msg) do {     \
	if (!(cond)) {                    \
		fprintf(stderr, err_msg); \
		exit(1);                  \
	}                                 \
} while (0)

static inline void parse_mode(CmdArgIter *iter) {
	static const RunMode DEFAULT = RUNMODE_SINGLEPLAYER;

	if (match(iter, "mp") || match(iter, "multiplayer")) {
		cmd_args.mode = RUNMODE_MULTIPLAYER;
		return;
	}
	if (match(iter, "sp") || match(iter, "singleplayer")) {
		cmd_args.mode = RUNMODE_SINGLEPLAYER;
		return;
	}
	cmd_args.mode = DEFAULT;
}

static inline void parse_mp_args(CmdArgIter *iter) {
	if (match(iter, "-ip")) {
		char *server_ip = next(iter);
		check_arg(server_ip != NULL, "IP address is missing!\n");
		cmd_args.server_ipdarr = server_ip;
	}

	if (match(iter, "-port")) {
		check_arg(cmd_args_next_u16(iter, &cmd_args.port), "Port Expected");
	}
}

static inline void parse_sp_arg(CmdArgIter *iter) {
	UNUSED(iter);
}

void args_parse(int argc, char *argv[]) {
	CmdArgIter iter = cmd_args_iter(argc, argv);
	cmd_args_next(&iter); // program
	
	parse_mode(&iter);

	while (cmd_args_has_next(&iter)) {
		switch (cmd_args.mode) {
		case RUNMODE_SINGLEPLAYER:
			parse_sp_arg(&iter);
			break;
		case RUNMODE_MULTIPLAYER:
			parse_mp_args(&iter);
			break;
		}
	}
}

static const char* RUNMODE_LABEL[] = {
	[RUNMODE_SINGLEPLAYER] = "SINGLEPLAYER",
	[RUNMODE_MULTIPLAYER] = "MULTIPLAYER",
};

void args_debug() {
	printf("CMD Args:\n");
	printf("  mode: %s\n", RUNMODE_LABEL[cmd_args.mode]);
	printf("    ip: %s\n", cmd_args.server_ipdarr);
	printf("  port: %d\n", cmd_args.port);
}

