#ifndef PALETTE_H
#define PALETTE_H

#include <raylib.h>

typedef struct {
	Color source;
	Color target;
} PaletteColor;

typedef struct {
	PaletteColor colors[64];
	int count;
} Palette;

Color auto_shade(Color base, Color shaded, Color new_base);
void palette_clear(Palette *palette);
int palette_push(Palette *palette, Color source, Color target);
int palette_push_shade(Palette *palette, int base_index, Color shaded_base);
Color palette_apply_pixel(Color color, Palette *palette);
void palette_apply(
	Color *pixels, int width, int height,
	Color *target_pixels, Palette *palette
);

#endif // PALETTE_H
