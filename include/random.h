#ifndef RANDOM_H
#define RANDOM_H


unsigned int random(unsigned int seed, int value);
unsigned int random2d(unsigned int seed, int x, int y);
unsigned int random3d(unsigned int seed, int x, int y, int z);
unsigned int random4d(unsigned int seed, int x, int y, int z, int w);

float randomf(unsigned int seed, int value);
float randomf2d(unsigned int seed, int x, int y);
float randomf3d(unsigned int seed, int x, int y, int z);
float randomf4d(unsigned int seed, int x, int y, int z, int w);



#endif // RANDOM_H
