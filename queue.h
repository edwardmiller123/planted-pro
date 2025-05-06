#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

#define MAX_QUEUE_SIZE 25

typedef struct queue
{
	uint32_t data[MAX_QUEUE_SIZE];
	uint32_t back;
	uint32_t pos;
	uint32_t size;
} queue;

void reset_queue(queue *q);

int fifo_add(queue *q, uint32_t elem);

int32_t fifo_get(queue *q);

#endif