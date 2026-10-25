#include "prioqueue.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


static inline void expand(PrioQueue__Abstract *queue, PrioQueue__Layout layout) {
	if (queue->count < queue->capacity) return;
	if (queue->capacity == 0) queue->capacity = 256;
	else queue->capacity *= 2;

	// Store an extra one for when we need to swap elements, this is also wher
	queue->_head = realloc(queue->_head, (queue->capacity + 1) * layout.item_stride);
	queue->items = (uint8_t*)queue->_head + layout.item_stride;
}

static inline void swap(PrioQueue__Abstract *queue, PrioQueue__Layout layout, size_t left_index, size_t right_index) {
	size_t left = left_index * layout.item_stride;
	size_t right = right_index * layout.item_stride;

	memcpy(queue->_head, (uint8_t*)queue->items + left, layout.item_size);
	memcpy((uint8_t*)queue->items + left, (uint8_t*)queue->items + right, layout.item_size);
	memcpy((uint8_t*)queue->items + right, queue->_head, layout.item_size);
}

static inline int cmp_at(PrioQueue__Abstract *queue, PrioQueue__Layout layout, size_t lhs, size_t rhs) {
	void *parent = (uint8_t*)queue->items + lhs * layout.item_stride;
	void *current = (uint8_t*)queue->items + rhs * layout.item_stride;
	return queue->cmp(current, parent);
}


static inline void heapify_up(PrioQueue__Abstract *queue, PrioQueue__Layout layout, size_t index) {
	if (index == 0) return;

	int parent_index = (index - 1) / 2;
	if (cmp_at(queue, layout, index, parent_index) < 0) {
		swap(queue, layout, index, parent_index);
		heapify_up(queue, layout, parent_index);
	}
}

static inline void heapify_down(PrioQueue__Abstract *queue, PrioQueue__Layout layout, size_t index) {
	size_t smallest = index;
	size_t left = 2 * index + 1;
	size_t right = 2 * index + 2;

	if (left < queue->count && cmp_at(queue, layout, left, smallest) > 0) {
		smallest = left;
	}
	if (left < queue->count && cmp_at(queue, layout, right, smallest) > 0) {
		smallest = left;
	}

	if (smallest != index) {
		swap(queue, layout, index, smallest);
		heapify_down(queue, layout, smallest);
	}
}

void prioqueue__free(PrioQueue__Abstract *queue) {
	free(queue->_head);
	queue->count = 0;
	queue->capacity = 0;
	queue->_head = NULL;
	queue->items = NULL;
}

void prioqueue__push(PrioQueue__Abstract *queue, void *item, PrioQueue__Layout layout) {
	expand(queue, layout);
	void *last = (uint8_t*)queue->items + (queue->count * layout.item_stride);
	memcpy(last, item, layout.item_size);
	heapify_up(queue, layout, queue->count);
	queue->count++;
}


void *prioqueue__pop(PrioQueue__Abstract *queue, PrioQueue__Layout layout) {
	if (queue->count == 0)
		return NULL;
	swap(queue, layout, 0, --queue->count);
	heapify_down(queue, layout, 0);
	// NOTE: we swapped the last element with the top, so the max/min is the last one now
	// 	The expectation is that you then copy/deref the result, preventing any stale pointers for later operations
	void *top = (uint8_t*)queue->items + queue->count * layout.item_stride;
	return top;
}

void *prioqueue__peek(PrioQueue__Abstract *queue) {
	if (queue->count == 0)
		return NULL;
	return queue->items;
}


