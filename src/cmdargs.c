#include "cmdargs.h"
#include "config.h"
#include <stdint.h>
#include <string.h>


CmdArgs cmd_args = {0};


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

// NOTE: This is an incredibly dumb, not designed args parser, 
// 	since I just wanted to get it out really quick
// 	Once this requires expansion, please look into how
// 	I am parsing cmdargs in ib/ect
void args_parse(int argc, char *argv[]) {
	CmdArgIter iter = {.args = argv, .count = argc, .index = 1};

	if (match(&iter, "server"))
		cmd_args.mode = RUNMODE_SERVER;

	while (has_next(&iter)) {
		char *curr = next(&iter);
		if (strcmp(curr, "-ip") == 0) {
			cmd_args.server_ipdarr = next(&iter);
			continue;
		}
	}
}

