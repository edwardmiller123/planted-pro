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
	buf->iterator_pos = 0;
	buf->word_count = 0;
	buf->word_capacity = RING_BUF_CAPACITY / sizeof(uint32_t);

	mem_zero(buf->data, RING_BUF_CAPACITY);
}

ring_buffer *init_ring_buffer()
{
	ring_buffer *buf = malloc(sizeof(ring_buffer));
	if (buf == NULL)
	{
		LOG(ERROR, "Failed to malloc ring buffer");
		return NULL;
	}

	ring_buffer_reset(buf);
	return buf;
}

// writes a byte into the buffer. Resets the iterator position
void ring_buffer_write_byte(ring_buffer *buf, uint8_t byte)
{
	if (buf->pos == RING_BUF_CAPACITY)
	{
		buf->pos = 0;
	}

	// case when the front has circled round and caught up with the back
	if (buf->pos == buf->back && buf->size > 0)
	{
		buf->back++;

		if (buf->back == RING_BUF_CAPACITY)
		{
			buf->back = 0;
		}
	}

	buf->data[buf->pos++] = byte;

	if (buf->size < RING_BUF_CAPACITY)
	{
		buf->size++;
	}

	buf->iterator_pos = buf->back;
}

uint8_t ring_buffer_read_byte(ring_buffer *buf, result_code *result)
{
	if (result != NULL)
	{
		*result = SUCCESS;
	}

	if (buf->size == 0)
	{
		if (result != NULL)
		{
			*result = EMPTY;
		}

		return 0;
	}

	if (buf->back == RING_BUF_CAPACITY)
	{
		buf->back = 0;
	}

	uint8_t byte = buf->data[buf->back++];

	buf->size--;

	buf->iterator_pos = buf->back;

	return byte;
}

void ring_buffer_write_word(ring_buffer *buf, uint32_t word)
{
	uint8_t *byte_ptr = (uint8_t *)&word;
	for (int i = 0; i < sizeof(uint32_t); i++)
	{
		ring_buffer_write_byte(buf, byte_ptr[i]);
	}

	if (buf->word_count < buf->word_capacity)
	{
		buf->word_count++;
	}
}

uint32_t ring_buffer_read_word(ring_buffer *buf, result_code *result)
{
	if (result != NULL)
	{
		*result = SUCCESS;
	}
	
	if (buf->size == 0)
	{
		if (result != NULL)
		{
			*result = EMPTY;
		}

		return 0;
	}

	uint32_t word;
	uint8_t *byte_ptr = (uint8_t *)&word;

	for (int i = 0; i < sizeof(uint32_t); i++)
	{
		byte_ptr[i] = ring_buffer_read_byte(buf, NULL);
	}

	// always assumes the data stored is all single bytes or full words not a mix
	if (buf->word_count > 0)
	{
		buf->word_count--;
	}

	return word;
}

uint8_t ring_buffer_next_byte(ring_buffer *buf)
{
	uint8_t val = buf->data[buf->iterator_pos];

	if (buf->pos > buf->back && buf->iterator_pos == buf->pos)
	{
		buf->iterator_pos = buf->back;
	}
	else if (buf->iterator_pos == (RING_BUF_CAPACITY - 1))
	{
		buf->iterator_pos = 0;
	}
	else
	{
		buf->iterator_pos++;
	}

	return val;
}

uint32_t ring_buffer_next_word(ring_buffer *buf)
{
	uint32_t word;
	uint8_t *byte_ptr = (uint8_t *)&word;

	for (int i = 0; i < sizeof(uint32_t); i++)
	{
		byte_ptr[i] = ring_buffer_next_byte(buf);
	}

	return word;
}
