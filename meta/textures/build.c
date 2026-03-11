#include <assert.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include "palette.h"

// NOTE: Maybe it would be easier to store this in the palette to begin with
//    For now, don't care
typedef struct {
	Color shade[64];
	int shade_c;
} ColorGroup;

#define assert_scanf(...) do {\
	if (scanf(__VA_ARGS__) != 1) {\
		fprintf(stderr, "scanf failed at line: %d\n", __LINE__);\
		abort();\
	}\
} while (0)


void export_asset(
	Color *base_colors, Image *new_image, 
	Color *new_colors, Palette *palette, 
	char *output
) {
	palette_apply(base_colors, new_image->width, new_image->height, new_colors, palette);
	for (int y = 0; y < new_image->height; y++) {
	for (int x = 0; x < new_image->width; x++) {
		ImageDrawPixel(new_image, x, y, new_colors[x + y * new_image->width]);
	}}
	ExportImage(*new_image, output);
}


Palette build_palette(ColorGroup *groups, int group_c) {
	Palette palette = {0};
	for (int i = 0; i < group_c; i++) {
		unsigned long color = 0;
		TraceLog(LOG_INFO, "Color[%d]: ", i);
		assert_scanf("%lu", &color);
		color = (color << 8) + 0xFF;
		TraceLog(LOG_DEBUG, "Color[%d]: %ul", i, color);
		Color new_main = GetColor(color);
		int shade_index = palette_push(&palette, groups[i].shade[0], new_main);
		for (int j = 1; j < groups[i].shade_c; j++) {
			palette_push_shade(&palette, shade_index, groups[i].shade[j]);
		}
	}
	return palette;
}

void build_texture(
	Image *base, Image *new_image,
	Color *base_colors, Color *new_colors,
	ColorGroup *groups, int group_c
) {
	char outfile[512] = {0};
	TraceLog(LOG_INFO, "Out: ");
	assert_scanf("%511s", outfile);

	Palette palette = build_palette(groups, group_c);

	export_asset(base_colors, new_image, new_colors, &palette, outfile);
}

void build_template() {
	char template[512] = {0};
	TraceLog(LOG_INFO, "Template Source: ");
	assert_scanf("%511s", template);
	Image base = LoadImage(template);

	ColorGroup groups[64] = {0};
	int group_c = 0;
	TraceLog(LOG_INFO, "Group Count: ");
	assert_scanf("%d", &group_c);
	for (int i = 0; i < group_c; i++) {
		TraceLog(LOG_INFO, "Group[%d].size: ", i);
		assert_scanf("%d", &groups[i].shade_c);
		TraceLog(LOG_INFO, "Group[%d].shades: ", i);
		for (int j = 0; j < groups[i].shade_c; j++) {
			unsigned long color = 0;
			assert_scanf("%lu", &color);
			color = (color << 8) + 0xFF;
			TraceLog(LOG_DEBUG, "Color[%d]: %ul", i, color);
			groups[i].shade[j] = GetColor(color);
		}
	}
	int texture_c = 0;
	TraceLog(LOG_INFO, "Texture Count: ");
	assert_scanf("%d", &texture_c);
	Image new_image = ImageCopy(base);
	Color *base_colors = LoadImageColors(base);
	Color *new_colors = LoadImageColors(new_image);
	for (int i = 0; i < texture_c; i++) {
		build_texture(
			&base, &new_image, 
			base_colors, new_colors, 
			groups, group_c
		);
	}
	UnloadImageColors(base_colors);
	UnloadImageColors(new_colors);
	UnloadImage(new_image);
	UnloadImage(base);
}


void build() {
	// SetTraceLogLevel(LOG_DEBUG);
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(150, 100, "Texture Generator BUILDER");
	SetTargetFPS(9999);

	int template_c = 0;
	TraceLog(LOG_INFO, "Template Count: ");
	assert_scanf("%d", &template_c);

	for (int i = 0; i < template_c; i++) {
		build_template();

		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("Generating textures...", 10, 10, 20, GREEN);
		EndDrawing();
	}
}
