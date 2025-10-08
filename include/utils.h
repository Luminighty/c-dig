#ifndef UTILS_H
#define UTILS_H

typedef enum {
	DIR_UP    = 1 << 0,
	DIR_DOWN  = 1 << 1,
	DIR_LEFT  = 1 << 2,
	DIR_RIGHT = 1 << 3,
} Direction;


#define dir_into_vec2(dir) ((Vec2){\
	.x = ((dir) & DIR_RIGHT) ? 1 : ((dir) & DIR_LEFT) ? -1 : 0,\
	.y = ((dir) & DIR_DOWN) ? 1 : ((dir) & DIR_UP) ? -1 : 0,\
})
#define vec2_into_dir(vec) (\
	((vec).x < 0 ? DIR_LEFT : (vec).x > 0 ? DIR_RIGHT : 0) |\
	((vec).y < 0 ? DIR_UP : (vec).y > 0 ? DIR_DOWN : 0)\
)


#endif //  UTILS_H
