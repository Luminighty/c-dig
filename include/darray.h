#ifndef VEC_H
#define VEC_H

#include <memory.h>

#define DynamicArray(Item) \
	struct { \
		Item *items; \
		size_t count; \
		size_t capacity; \
	}


#define da_push(vec, value)\
	do {\
	if ((vec).count >= (vec).capacity) {\
		if ((vec).capacity == 0) (vec).capacity = 256;\
		else (vec).capacity *= 2;\
		(vec).items = realloc((vec).items, (vec).capacity * sizeof(*((vec).items)));\
	}\
	(vec).items[(vec).count++] = value;\
	} while (0)


#define da_swapremove(vec, index) do {                     \
	(vec).items[index] = (vec).items[--((vec).count)]; \
} while(0)
#define da_pop(vec) (vec).items[--(vec).count]
#define da_clear(vec) do { (vec).count = 0; } while(0)
#define da_free(vec) do {                       \
	(vec).count = 0; (vec).capacity = 0;        \
	free((vec).items); (vec).items = NULL;      \
} while(0) 
#define da_last(vec) (vec).items[(vec).count - 1]

#endif // VEC_H
