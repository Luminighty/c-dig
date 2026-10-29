#include "client/input.h"
#include "config.h"
#include "linalg.h"
#include <raylib.h>
#include <stdio.h>

Input input = {0};


static Vec2 input_mouse_to_game();
static Vec2 input_mouse_to_ui();

void input_update() {
	input.movement.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	input.movement.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

	input.ui_mouse = input_mouse_to_ui();
	input.mouse = input_mouse_to_game();
	Vec2 mouse_tile = vec2_div(input.mouse, TILE_PIXEL_SIZE);
	input.mouse_tile.x = mouse_tile.x;
	input.mouse_tile.y = mouse_tile.y;

	input.mouse_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	input.mouse_held = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

	input.digging = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	input.jump = IsKeyPressed(KEY_SPACE);
	input.randomize_player = IsKeyPressed(KEY_R);

	input.hot_reload = IsKeyPressed(KEY_F5);

	for (int i = 0; i < 10; i++) {
		input.slot[i] = IsKeyPressed(KEY_ZERO + i);
	}
}

static Vec2 input_mouse_to_game() {
	Vector2 _pos = GetMousePosition();
	Vec2 pos = {{_pos.x, _pos.y}};
	Rect2 viewport = game_viewport();
	pos = vec2_sub(pos, viewport.pos);
	pos = vec2_div(pos, GAME_SCALE);
	return pos;
}


static Vec2 input_mouse_to_ui() {
	Vector2 _pos = GetMousePosition();
	Vec2 pos = {{_pos.x, _pos.y}};
	pos = vec2_div(pos, UI_SCALE);
	return pos;
}

