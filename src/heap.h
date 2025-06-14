#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdbool.h>

#include "list.h"

#define MAX_HEAP_SIZE 8192

typedef struct __attribute__ ((packed)) __attribute__((aligned(16))) heap {
	uint8_t data[MAX_HEAP_SIZE];
	list * free_blocks;
	uint8_t * top;
	uint8_t * bottom;

} heap;

void init_heap();

void *malloc(uint32_t size);

// frees the given block of allocated memory. It is the callers responsability to ensure
// the alloc_mem is non NULL and originally came from a malloc.
void free(void *alloc_mem);

#endif