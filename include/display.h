#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"


#define ZINDEX_SKIP 0

typedef char ZIndex;

typedef struct {
	char glyph;
	int fg;
	int bg;
} Glyph;

#define glyph(_glyph, _fg, _bg) (Glyph){ .glyph=(_glyph), .fg=(_fg), .bg=(_bg) }


typedef struct {
	Glyph glyphs[SCREEN_HEIGHT][SCREEN_WIDTH];
	ZIndex z_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
} Display;


void display_init();
void display_destroy();

void display_render();
void display_clear();
void display_putchar(int x, int y, Glyph glyph, ZIndex z_index);


#endif // DISPLAY_H
