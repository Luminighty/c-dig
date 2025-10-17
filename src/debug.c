#include "debug.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED

#define LOG_LENGTH 256
#define MAX_LOGS 32

static char logs[MAX_LOGS][LOG_LENGTH] = {0};
static int logs_c = 0;

static const int POINT_SIZE = 3;


void draw_point(int x, int y, Color color) {
	DrawLine(x - POINT_SIZE, y - POINT_SIZE, x + POINT_SIZE, y + POINT_SIZE, color);
	DrawLine(x + POINT_SIZE, y - POINT_SIZE, x - POINT_SIZE, y + POINT_SIZE, color);
}

void draw_box(int x, int y, int w, int h, Color color) {
	DrawRectangleLines(x, y, w, h, color);
	draw_point(x + w / 2, y + h / 2, color);
}

inline void debug_start() { logs_c = 0; }

void debug_end() {
	DrawRectangle(0, 0, 100, logs_c * 15, ColorAlpha(BLACK, 0.5));
	for (int i = 0; i < logs_c; i++) {
		DrawText(logs[i], 5, i * 15, 0, WHITE);
	}
}

void debug_log(const char* fmt, ...) {
	if (logs_c >= MAX_LOGS) return;
	va_list args;
	va_start(args, fmt);
	vsnprintf(logs[logs_c++], LOG_LENGTH, fmt, args);
	va_end(args);
}

#else

void draw_point(int x, int y, Color color) {}
void draw_box(int x, int y, int w, int h, Color color) {}
inline void debug_start() { logs_c = 0; }
void debug_end() {}
void debug_log(const char* fmt, ...) {}

#endif
