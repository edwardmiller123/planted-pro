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

#endif