#include "palette.h"
#include <math.h>
#include <raylib.h>

static float float_clamp(float value, float min, float max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

static float float_mod(float dividend, float divisor) {
	while (dividend > divisor)
		dividend -= divisor;
	while (dividend < 0.f)
		dividend += divisor;
	return dividend;
}

Color auto_shade(Color base, Color shaded, Color new_base) {
	Vector3 base_hsv = ColorToHSV(base);
	Vector3 shaded_hsv = ColorToHSV(shaded);
	Vector3 delta = {
		shaded_hsv.x - base_hsv.x,
		shaded_hsv.y - base_hsv.y,
		shaded_hsv.z - base_hsv.z,
	};
	Vector3 new_base_hsv = ColorToHSV(new_base);
	return ColorFromHSV(
		float_mod(new_base_hsv.x + delta.x, 360.f),
		float_clamp(new_base_hsv.y + delta.y, 0.f, 1.f),
		float_clamp(new_base_hsv.z + delta.z, 0.f, 1.f)
	);
}


void palette_clear(Palette *palette) {
	palette->count = 0;
}


int palette_push(Palette *palette, Color source, Color target) {
	palette->colors[palette->count].source = source;
	palette->colors[palette->count].target = target;
	return palette->count++;
}


int palette_push_shade(Palette *palette, int base_index, Color shaded_base) {
	Color shaded = auto_shade(
		palette->colors[base_index].source, 
		shaded_base, 
		palette->colors[base_index].target
	);
	return palette_push(palette, shaded_base, shaded);
}


Color palette_apply_pixel(Color color, Palette *palette) {
	for (int j = 0; j < palette->count; j++) {
		if (ColorIsEqual(color, palette->colors[j].source)) {
			return palette->colors[j].target;
		}
	}
	return color;
}


void palette_apply(
	Color *pixels, int width, int height,
	Color *target_pixels, Palette *palette
) {
	int size = width * height;

	for (int i = 0; i < size; i++) {
		Color new_color = palette_apply_pixel(pixels[i], palette);
		target_pixels[i] = new_color;
		// int x = i % width;
		// int y = i / width;
		// ImageDrawPixel(target, x, y, new_color);
	}
}

