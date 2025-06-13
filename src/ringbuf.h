#include <stdint.h>

#define RING_BUF_CAPACITY 64

typedef struct ring_buffer {
	uint8_t data[128];
	int8_t back;
	uint8_t pos;
	uint8_t size;
} ring_buffer;

void ring_buffer_reset(ring_buffer * buf);

ring_buffer * init_ring_buffer();

void ring_buffer_write(ring_buffer *buf, uint8_t byte);

uint8_t ring_buffer_read(ring_buffer *buf);