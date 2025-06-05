#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

#define DEFAULT_QUEUE_CAPACITY 25

typedef struct queue
{
	uint8_t * data;
	uint32_t back;
	uint32_t pos;
	uint32_t size;
	uint32_t capacity;
} queue;

void queue_reset(queue *q);

queue * init_queue(uint32_t capacity);

int fifo_add(queue *q, uint32_t elem);

int32_t fifo_get(queue *q);

#endif