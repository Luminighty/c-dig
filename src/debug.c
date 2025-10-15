#include "debug.h"


static const int POINT_SIZE = 5;

void draw_point(int x, int y, Color color) {
	DrawLine(x - POINT_SIZE, y - POINT_SIZE, x + POINT_SIZE, y + POINT_SIZE, color);
	DrawLine(x + POINT_SIZE, y - POINT_SIZE, x - POINT_SIZE, y + POINT_SIZE, color);
}

void draw_box(int x, int y, int w, int h, Color color) {
	DrawRectangleLines(x, y, w, h, color);
	draw_point(x + w / 2, y + h / 2, color);
}
