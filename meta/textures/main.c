#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

typedef enum {
	MODE_BUILD,
	MODE_INTERACTIVE,
} RunMode;


RunMode get_runmode(int argc, char *argv[]) {
	if (argc < 2)
		return MODE_INTERACTIVE;
	if (strcmp(argv[1], "help") == 0) {
		printf("Usage: %s (build | interact | help)\n", argv[0]);
		exit(0);
	}
	if (strcmp(argv[1], "build") == 0)
		return MODE_BUILD;
	if (strcmp(argv[1], "interact") == 0)
		return MODE_INTERACTIVE;
	printf("Usage: %s (build | interact | help)\n", argv[0]);
	exit(1);
}

#include "interactive.h"
#include "build.h"

int main(int argc, char *argv[]) {
	RunMode mode = get_runmode(argc, argv);
	switch (mode) {
		case MODE_BUILD: build(); break;
		case MODE_INTERACTIVE: interactive(); break;
	}
}

#include "interactive.c"
#include "build.c"
#include "palette.c"
