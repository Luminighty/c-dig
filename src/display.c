#include "display.h"
#include "palette.h"
#include <raylib.h>

static Display display = {0};
static Texture2D ascii;


#define foreach(x, y)\
	for(int y = 0; y < SCREEN_HEIGHT; y++)\
	for(int x = 0; x < SCREEN_WIDTH; x++)

static inline int is_on_screen(int x, int y) { 
	return x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT;
}

void display_init() {
	ascii = LoadTexture("assets/pastiche_8x8.png"); 
}

void display_destroy() {
	UnloadTexture(ascii);
}


void display_render() {
	static const int TILE = 8;
	foreach(x, y) {
		char g = display.glyphs[y][x].glyph;
		int sx = g & 0xF;
		int sy = (g >> 4) & 0xF;

		Color fg = color_into_raylib(display.glyphs[y][x].fg);
		Color bg = color_into_raylib(display.glyphs[y][x].bg);

		Rectangle source = { sx * TILE, sy * TILE, TILE, TILE};
		Rectangle dest = { x * TILE, y * TILE, TILE, TILE};
		Vector2 origin = {0};
		DrawRectangleRec(dest, bg);
		DrawTexturePro(ascii, source, dest, origin, 0, fg);
	}
}


void display_clear() {
	foreach(x, y) {
		display.glyphs[y][x].fg = 0;
		display.glyphs[y][x].bg = 0;
		display.z_buffer[y][x] = 0;
	}
}


void display_putchar(int x, int y, Glyph glyph, ZIndex z_index) {
	if (!is_on_screen(x, y))
		return;
	if (z_index != ZINDEX_SKIP && z_index < display.z_buffer[y][x])
		return;
	if (z_index != ZINDEX_SKIP)
		display.z_buffer[y][x] = z_index;
	
	display.glyphs[y][x] = glyph;
}

void display_putstr(int x, int y, const char* message, int fg, int bg, ZIndex z_index) {
	int offset = 0;
	char c;
	while ((c = message[offset])) {
		display_putchar(x + offset, y, (Glyph){.glyph=c, .fg=fg, .bg=bg}, z_index);
		offset++;
	}
}

