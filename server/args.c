#include "server/args.h"
#include "cmd_parser.h"
#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

CmdArgs cmd_args = {
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


static inline void parse_arg(CmdArgIter *iter) {
	if (match(iter, "-port")) {
		check_arg(cmd_args_next_u16(iter, &cmd_args.port), "Port Expected");
	}
}


void args_parse(int argc, char *argv[]) {
	CmdArgIter iter = cmd_args_iter(argc, argv);
	cmd_args_next(&iter); // program

	while (cmd_args_has_next(&iter)) {
		parse_arg(&iter);
	}
}


void args_debug() {
	printf("CMD Args:\n");
	printf("  port: %d\n", cmd_args.port);
}

