#include <stdint.h>

#include "queue.h"
#include "utils.h"
#include "logger.h"
#include "heap.h"

// TODO: should make queue capacity fixed size with statically allocated memory
void queue_reset(queue *q)
{
	q->size = 0;
	q->back = 0;
	q->pos = 0;
	q->iterator_pos = 0;
	logger(DEBUG, "Queue reset");
}

queue *init_queue(uint32_t capacity)
{
	uint8_t *data = malloc(capacity * sizeof(uint32_t));
	if (data == NULL)
	{
		logger(ERROR, "Failed to allocate heap memory for queue capacity");
		return NULL;
	}

	queue *q = malloc(sizeof(queue));
	if (q == NULL)
	{
		logger(ERROR, "Failed to allocate heap memory for queue");
		return NULL;
	}

	q->data = data;
	q->mem_capacity = capacity;

	queue_reset(q);

	uint32_t args[] = {capacity};
	loggerf(INFO, "Initialised queue with capacity $", args, 1, NULL, 0);
	return q;
}

int fifo_add(queue *q, uint32_t elem)
{
	if (q->pos * sizeof(uint32_t) == q->mem_capacity)
	{
		logger(ERROR, "Queue full");
		queue_reset(q);
		return -1;
	}

	// a little janky but using ptr arithmetic allows us to dynamically allocate queue size on initialisation
	uint32_t mem_pos = q->pos * sizeof(uint32_t);
	*(uint32_t *)((uint32_t)q->data + mem_pos) = elem;

	q->size++;
	q->pos++;

	uint32_t args[] = {elem};
	loggerf(DEBUG, "Added $ to queue", args, 1, NULL, 0);

	return 0;
}

int32_t fifo_get(queue *q)
{
	if (q->size == 0)
	{
		logger(WARNING, "Cant remove element from empty queue");
		return -1;
	}

	uint32_t mem_pos = q->back * sizeof(uint32_t);
	uint32_t val = *(uint32_t *)((uint32_t)q->data + mem_pos);

	q->back++;
	q->size--;

	uint32_t args[] = {q->data[q->back - 1]};
	loggerf(DEBUG, "Removed $ from queue", args, 1, NULL, 0);

	if (q->back == q->mem_capacity)
	{
		logger(WARNING, "Queue capacity reached");
		queue_reset(q);
	}

	return (int32_t)val;
}

uint32_t queue_next(queue *q)
{
	uint32_t val = *(uint32_t *)(q->data + (q->iterator_pos++ * sizeof(uint32_t)));

	if (q->iterator_pos > q->size)
	{
		q->iterator_pos = 0;
	}

	return val;
}