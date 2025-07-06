#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include "utils.h"

// This must be a multiple of 4
#define RING_BUF_CAPACITY 128

// circular buffer. Reads and writes should eitehr be as uint8_t or uint32_t.
// Using both in the same buffer is undefined
typedef struct ring_buffer {
	uint8_t back;
	uint8_t pos;
	uint8_t size;
	uint8_t iterator_pos;
	uint8_t word_count;
	uint8_t word_capacity;
	uint8_t data[RING_BUF_CAPACITY];
} ring_buffer;

void ring_buffer_reset(ring_buffer * buf);

ring_buffer * init_ring_buffer();

void ring_buffer_write_byte(ring_buffer *buf, uint8_t byte);

// Reads a byte into the buffer. Resets the iterator position. Sets the given result code
// if it is not NULL.
uint8_t ring_buffer_read_byte(ring_buffer *buf, result_code *result);

// write the given word into the buffer.
void ring_buffer_write_word(ring_buffer *buf, uint32_t word);

uint32_t ring_buffer_read_word(ring_buffer *buf, result_code *result);

// iterate through the buffer without removing elements. Iteration is reset by a read or writ etoi the buffer.
uint8_t ring_buffer_next_byte(ring_buffer *buf);

uint32_t ring_buffer_next_word(ring_buffer *buf);

#endif