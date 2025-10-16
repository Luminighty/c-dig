#ifndef INPUT_H
#define INPUT_H

#include "linalg.h"
#include <stdbool.h>


typedef struct {
	Vec2 movement;
	Vec2 mouse;
	Vec2i mouse_tile;
	bool clicked;
	bool digging;
	bool jump;
	bool randomize_player;
} Input;

extern Input input;

void input_update();


#endif // INPUT_H
