#include "input.h"
#include <raylib.h>

Input input = {0};

void input_update() {
	input.movement.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	input.movement.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	input.digging = IsKeyDown(KEY_LEFT_SHIFT);
	input.jump = IsKeyPressed(KEY_SPACE);
	input.randomize_player = IsKeyPressed(KEY_R);
}

