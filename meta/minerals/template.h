#include <stdint.h>

typedef struct {
	uint32_t color;
} MineralData;

#define mineral_color(tile)   (mineral_data[tile].color)
