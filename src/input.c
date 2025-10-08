#include "input.h"
#include <raylib.h>

Input input = {0};

void input_update() {
	input.movement.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
	input.movement.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
	input.digging = IsKeyDown(KEY_LEFT_SHIFT);
	input.jump = IsKeyPressed(KEY_SPACE);
}

