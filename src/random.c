#include "random.h"

#define PRIME1 198491317
#define PRIME2 6542989
#define PRIME3 357239

static unsigned int inner_state = 0;

// Code from https://gist.github.com/kevinmoran/0198d8e9de0da7057abe8b8b34d50f86
unsigned int srandom(unsigned int seed, int value) {
	static const unsigned int SQ5_BIT_NOISE1 = 0xd2a80a3f;
	static const unsigned int SQ5_BIT_NOISE2 = 0xa884f197;
	static const unsigned int SQ5_BIT_NOISE3 = 0x6C736F4B;
	static const unsigned int SQ5_BIT_NOISE4 = 0xB79F3ABB;
	static const unsigned int SQ5_BIT_NOISE5 = 0x1b56c4f5;

	unsigned int mangled = (unsigned int) value;
	mangled *= SQ5_BIT_NOISE1;
	mangled += seed;
	mangled ^= (mangled >> 9);
	mangled += SQ5_BIT_NOISE2;
	mangled ^= (mangled >> 11);
	mangled *= SQ5_BIT_NOISE3;
	mangled ^= (mangled >> 13);
	mangled += SQ5_BIT_NOISE4;
	mangled ^= (mangled >> 15);
	mangled *= SQ5_BIT_NOISE5;
	mangled ^= (mangled >> 17);
	return mangled;
}


unsigned int srandom2d(unsigned int seed, int x, int y) {
	return srandom(seed, x + (PRIME1 * y));
}

unsigned int srandom3d(unsigned int seed, int x, int y, int z) {
	return srandom(seed, x + (PRIME1 * y) + (PRIME2 * z));
}

unsigned int srandom4d(unsigned int seed, int x, int y, int z, int w) {
	return srandom(seed, x + (PRIME1 * y) + (PRIME2 * z) + (PRIME3 * w));
}


#define ONE_OVER_MAX_UINT (1.0 / (double)0xFFFFFFFF)
#define to_float(value) (float)(ONE_OVER_MAX_UINT * (double)(value))

float srandomf(unsigned int seed, int value)	 { return to_float(srandom(seed, value)); }
float srandomf2d(unsigned int seed, int x, int y) { return to_float(srandom2d(seed, x, y)); }
float srandomf3d(unsigned int seed, int x, int y, int z)		{ return to_float(srandom3d(seed, x, y, z)); }
float srandomf4d(unsigned int seed, int x, int y, int z, int w)	{ return to_float(srandom4d(seed, x, y, z, w)); }


int random_range(int min, int max) {
	return (random(0, inner_state++) % (max - min)) + min;
}
