#ifndef DEBUG_H
#define DEBUG_H

#include <raylib.h>


void draw_point(int x, int y, Color color);
void draw_box(int x, int y, int w, int h, Color color);

void debug_start();
void debug_end();
void debug_log(const char* message, ...);

#endif // DEBUG_H
