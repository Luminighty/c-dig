#include "input.h"
#include "config.h"
#include "linalg.h"
#include <raylib.h>
#include <stdio.h>

Input input = {0};

void input_update() {
	input.movement.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	input.movement.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	input.mouse.x = GetMousePosition().x / WINDOW_SCALE;
	input.mouse.y = GetMousePosition().y / WINDOW_SCALE;
	input.clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	input.mouse_tile.x = input.mouse.x / TILE_PIXEL_SIZE;
	input.mouse_tile.y = input.mouse.y / TILE_PIXEL_SIZE;
	input.digging = IsKeyDown(KEY_LEFT_SHIFT);
	input.jump = IsKeyPressed(KEY_SPACE);
	input.randomize_player = IsKeyPressed(KEY_R);
}

