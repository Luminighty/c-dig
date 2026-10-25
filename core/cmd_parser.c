#include "cmd_parser.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


CmdArgIter cmd_args_iter(int argc, char *argv[]) {
	return (CmdArgIter){
		.args = argv,
		.count = argc,
		.index = 0
	};
}

char *cmd_args_next(CmdArgIter *iter) {
	if (iter->index >= iter->count)
		return NULL;
	return iter->args[iter->index++];
}

bool cmd_args_has_next(CmdArgIter *iter) {
	return iter->index < iter->count;
}

bool cmd_args_match(CmdArgIter *iter, const char* arg) {
	if (iter->index >= iter->count)
		return false;
	if (strcmp(cmd_args_peek(iter), arg) == 0) {
		cmd_args_next(iter);
		return true;
	}
	return false;
}

char *cmd_args_peek(CmdArgIter *iter) {
	if (iter->index >= iter->count)
		return NULL;
	return iter->args[iter->index];
}

#define next_num_impl(fn_name, type)                                      \
	bool fn_name (CmdArgIter *iter, type *result) {                   \
		char *arg = cmd_args_next(iter);                          \
		if (arg == NULL)                                          \
			return false;                                     \
		int num = atoi(arg);                                      \
		bool is_actually_zero = strlen(arg) == 1 && arg[0] == '0';\
		if (num == 0 && !is_actually_zero)                        \
			return false;                                     \
		*result = num;                                            \
		return true;                                              \
	}

next_num_impl(cmd_args_next_u16, uint16_t)
