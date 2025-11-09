#include <stdint.h>

#include "heap.h"
#include "list.h"
#include "utils.h"
#include "logger.h"

// The heap consists of a 4kib array. We keep track of the top in a variable and "new" free blocks
// in a linked list. For each block allocated we allocate 4 bytes before the returned pointer to store the block size. When a block is
// freed, we use the first 12 bytes of the block to store a list node for the list of free blocks. When allocating memory we first check the free blocks
// in the list to see if one is big enough and if not we allocate on the top of the heap and increment the marker. To free a block we simply add
// it to the list of free blocks unless it is the block before the top in which case we simply move the top marker back.
// We define the start of a block where the size meta data begins.

// TODO: Maybe try to reduce memory fragmentation

#define META_DATA_SIZE 4
#define ALLIGNMENT 16

list free_blocks;
heap h;

void init_heap()
{
	init_list(&free_blocks);
	h.free_blocks = &free_blocks;
	h.bottom = &h.data[0];
	h.top = &h.data[0];

	[[maybe_unused]] uint32_t args[] = {MAX_HEAP_SIZE};
	LOGF(INFO, "Initialised heap. Max size: $", args, 1, NULL, 0);
}

void *malloc(uint32_t size)
{
	if (size == 0)
	{
		LOG(ERROR, "Tried to allocate 0 bytes");
		return NULL;
	}

	if ((size + META_DATA_SIZE) % ALLIGNMENT != 0)
	{
		size += ALLIGNMENT - (((size + META_DATA_SIZE) % ALLIGNMENT));
	}

	const uint8_t *heap_max = (uint8_t *)((uint32_t)h.bottom + MAX_HEAP_SIZE);
	// first try to allocate in previously freed blocks.
	uint8_t *current_block = (uint8_t *)h.free_blocks->head->data;
	for (int i = 0; i < h.free_blocks->size; i++)
	{
		node *free_block_node = (node *)(current_block + META_DATA_SIZE);

		if (*(uint32_t *)current_block >= size)
		{
			list_remove(h.free_blocks, free_block_node);
			*(uint32_t *)current_block = size;

			[[maybe_unused]] uint32_t args[] = {size, (uint32_t)current_block};
			LOGF(DEBUG, "Allocated $ bytes on heap at free block address: $ ", args, 2, NULL, 0);

			return (void *)(current_block + META_DATA_SIZE);
		}

		current_block = (uint8_t *)free_block_node->next->data;
	}

	if (h.top >= heap_max)
	{
		LOG(ERROR, "Heap memory limit reached");
		return NULL;
	}

	// if none are large enough then allocate on the top of the heap
	current_block = h.top;
	*(uint32_t *)current_block = size;

	h.top += size + META_DATA_SIZE;

	uint32_t usage = (uint32_t)h.top - (uint32_t)h.bottom;
	[[maybe_unused]] uint32_t args[] = {size + META_DATA_SIZE, (uint32_t)current_block, (uint32_t)h.top, usage};
	LOGF(DEBUG, "Allocated $ bytes on heap at new block address $. Heap top: $, Usage: $ bytes", args, 4, NULL, 0);

	return (void *)(current_block + META_DATA_SIZE);
}

void free(void *alloc_mem)
{
	uint8_t *block = (uint8_t *)((uint32_t)alloc_mem - META_DATA_SIZE);
	uint32_t size = *(uint32_t *)block;

	// if the given block is at the top already then just decrease the top of the heap
	if ((uint32_t)alloc_mem + size == (uint32_t)h.top)
	{
		h.top = block;

		uint32_t usage = (uint32_t)h.top - (uint32_t)h.bottom;
		[[maybe_unused]] uint32_t args[] = {(uint32_t)block, usage, (uint32_t)h.top};
		LOGF(DEBUG, "Freed heap block $. Heap usage decreased to $. Heap top: $", args, 3, NULL, 0);
		return;
	}

	// otherwise add the block to the list of freed ones. Store the free block node
	// in the block itself since its not being used.
	node free_block_node = {.data = (uint32_t)block};
	byte_copy((uint8_t *)&free_block_node, (uint8_t *)alloc_mem, sizeof(node));
	list_add(h.free_blocks, (node *)alloc_mem);

	[[maybe_unused]] uint32_t args[] = {(uint32_t)block};
	LOGF(DEBUG, "Freed heap block $. Added block to free list", args, 3, NULL, 0);
}