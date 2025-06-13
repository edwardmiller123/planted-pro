#include <stdint.h>

#include "ringbuf.h"
#include "logger.h"
#include "heap.h"
#include "utils.h"

void ring_buffer_reset(ring_buffer *buf)
{
	buf->back = 0;
	buf->pos = 0;
	buf->size = 0;

	mem_zero(buf->data, RING_BUF_CAPACITY);
}

ring_buffer *init_ring_buffer()
{
	ring_buffer *buf = malloc(sizeof(ring_buffer));
	if (buf == NULL)
	{
		logger(ERROR, "Failed to malloc ring buffer");
		return NULL;
	}

	ring_buffer_reset(buf);
	return buf;
}

void ring_buffer_write(ring_buffer *buf, uint8_t byte)
{
	if (buf->pos == RING_BUF_CAPACITY)
	{
		buf->pos = 0;
	}

	// case when the front has circled round and caught up with the back
	if (buf->pos == buf->back && buf->size > 0)
	{
		buf->back++;
	}

	buf->data[buf->pos++] = byte;
	buf->size++;
}

uint8_t ring_buffer_read(ring_buffer *buf)
{
	if (buf->size == 0)
	{
		return 0;
	}

	uint8_t byte = buf->data[buf->back++];
	buf->size--;
	return byte;
}