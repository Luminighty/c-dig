#include "palette.h"
#include <raylib.h>
#include <stdio.h>
#include "raygui.h"


void palette_ore(Palette *palette, Color main) {
	Color BASE_MAIN = GetColor(0xad8c76ff);
	Color BASE_SHADED = GetColor(0x7b6250ff);
	palette_clear(palette);
	int main_color = palette_push(palette, BASE_MAIN, main);
	palette_push_shade(palette, main_color, BASE_SHADED);
}


void interactive() {
	int SCALE = 10;
	int WIDTH = 400;
	int HEIGHT = 400;

	InitWindow(WIDTH, HEIGHT, "Texture Generator");
	SetTargetFPS(60);

	// TODO: Find templates maybe through the lua build?
	Image base = LoadImage("./assets/templates/tile_ore.png");

	Color *base_colors = LoadImageColors(base);
	Color *new_colors = LoadImageColors(base);

	Texture texture = LoadTextureFromImage(base);

	Palette palette = {0};
	Vector3 new_main_hsv = ColorToHSV(RED);
	Color previous_main = {0};

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_ENTER)) {
			unsigned int color = ColorToInt(previous_main);
			// NOTE: Remove transparency for ease
			color = color >> 8;
			printf("Current Color: 0x%x\n", color);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		GuiColorPickerHSV((Rectangle){ 10, 10, 100, 100}, "Color", &new_main_hsv);
		Color new_main = ColorFromHSV(new_main_hsv.x, new_main_hsv.y, new_main_hsv.z);

		if (!ColorIsEqual(previous_main, new_main)) {
			palette_ore(&palette, new_main);
			palette_apply(base_colors, base.width, base.height, new_colors, &palette);
			UpdateTexture(texture, new_colors);
			previous_main = new_main;
		}

		Rectangle src = {0, 0, base.width, base.height};
		Rectangle dest = {
			(WIDTH - base.width * SCALE) / 2.0f,
			(HEIGHT - base.height * SCALE) / 2.0f,
			base.width * SCALE,
			base.height * SCALE
		};
		DrawTexturePro(texture, src, dest, (Vector2){0}, 0.0f, WHITE);
		EndDrawing();
	}
}

