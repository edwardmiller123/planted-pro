#include <stdint.h>

#include "utils.h"
#include "logger.h"

#include "queue.h"

void reset_queue(queue *q)
{
	q->size = 0;
	q->back = 0;
	q->pos = 0;
	mem_zero(q->data, MAX_QUEUE_SIZE);

	logger(DEBUG, "Queue resetting.");
}

int fifo_add(queue *q, uint32_t elem)
{
	if (q->size = MAX_QUEUE_SIZE)
	{
		logger(ERROR, "Queue full");
		reset_queue(q);
		return -1;
	}

	q->data[q->pos++] = elem;

	q->size++;

	uint32_t args[] = {elem};
	loggerf(DEBUG, "Added $ to queue", args, 1);

	return 0;
}

uint32_t fifo_get(queue *q) {
	if (q->size == 0) {
		logger(ERROR, "Queue empty");
		return 0;
	}

	uint32_t val = q->data[q->back++];

	q->size--;

	uint32_t args[] = {q->data[q->back - 1]};
	loggerf(DEBUG, "Removed $ from queue", args, 1);

	if (q->back == MAX_QUEUE_SIZE) {
		reset_queue(q);
	}

	return val;
}