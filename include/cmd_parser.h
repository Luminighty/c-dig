#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <stdbool.h>
#include <stdint.h>


typedef struct {
	char **args;
	int count;
	int index;
} CmdArgIter;


CmdArgIter cmd_args_iter(int argc, char *argv[]);
char *cmd_args_next(CmdArgIter *iter);
bool cmd_args_has_next(CmdArgIter *iter);

bool cmd_args_match(CmdArgIter *iter, const char* arg);
char *cmd_args_peek(CmdArgIter *iter);

bool cmd_args_next_u16(CmdArgIter *iter, uint16_t *result);


#endif // CMD_PARSER_H
