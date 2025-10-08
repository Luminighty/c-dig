#ifndef INPUT_H
#define INPUT_H

#include "math.h"
#include <stdbool.h>


typedef struct {
	Vec2 movement;
	bool digging;
	bool jump;
} Input;

extern Input input;

void input_update();


#endif // INPUT_H
