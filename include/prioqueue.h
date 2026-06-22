#ifndef HEAPTREE_H
#define HEAPTREE_H

#include <stddef.h>


#define PrioQueue(Item) struct {                             \
		Item *items;                                 \
		size_t count;                                \
		size_t capacity;                             \
		int (*cmp)(Item*, Item*);                    \
		/* This value is used for swapping items */  \
		Item *head;                                  \
	}

#define prioqueue_create(_cmp) { .cmp = _cmp }


#define prioqueue_free(queue) prioqueue__free((PrioQueue__Abstract*)(queue));

#define prioqueue_push(queue, item)            \
	prioqueue__push(                       \
		(PrioQueue__Abstract*)(queue), \
		&item,                         \
		prioqueue__layout(queue)       \
	)

#define prioqueue_pop(queue) \
	*((prioqueue__cast(queue))                     \
		prioqueue__pop(                        \
			(PrioQueue__Abstract*)(queue), \
			prioqueue__layout(queue)       \
		)                                      \
	)

#define prioqueue_peek(queue)                          \
	*((prioqueue__cast(queue))                     \
		prioqueue__peek(                       \
			(PrioQueue__Abstract*)(queue)  \
		)                                      \
	)


//
//  This is the start of the private implementation detail section
//  	When using this API, please use the functions above
//


// NOTE: __typeof__ is a C23 feature
#define prioqueue__cast(queue) __typeof__(*(queue)->items)*
#define prioqueue__word_align(size) (((size) + sizeof(uintptr_t) - 1)/sizeof(uintptr_t)*sizeof(uintptr_t))
#define prioqueue__item_size(queue) sizeof(*(queue)->items)

#define prioqueue__layout(queue) ((PrioQueue__Layout){    \
		.item_size = prioqueue__item_size(queue), \
		.item_stride = prioqueue__word_align(      \
			prioqueue__item_size(queue)       \
		)                                         \
	})

typedef struct {
	size_t item_size;
	size_t item_stride;
} PrioQueue__Layout;

typedef struct {
	void *items;
	size_t count;
	size_t capacity;
	int (*cmp)(void*, void*);
	void *_head;
} PrioQueue__Abstract;

void prioqueue__free(PrioQueue__Abstract *queue);
void prioqueue__push(PrioQueue__Abstract *queue, void *item, PrioQueue__Layout layout);
void *prioqueue__pop(PrioQueue__Abstract *queue, PrioQueue__Layout layout);
void *prioqueue__peek(PrioQueue__Abstract *queue);


#endif // HEAPTREE_H
