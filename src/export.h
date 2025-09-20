#ifndef EXPORT_H
#define EXPORT_H

#include <stdint.h>
#include <stdbool.h>

#include "ringbuf.h"

typedef struct data_point {
	uint32_t ts;
	uint8_t light_percent;
	uint8_t water_percent;
	uint8_t battery_percent;
} data_point;

// the exporter sends the contense of the export queue over USART1 as a JSON string when requested.
// Every time the time interval is passed we record the sensor values and discard the earliest recorded value if the queue is full
typedef struct exporter {
	ring_buffer * export_buf;
	uint16_t poll_interval;
	uint32_t last_read_ts;
	uint16_t data_point_limit;
	bool send_data;

} exporter;

exporter *init_exporter(uint16_t poll_interval, uint16_t data_point_count);

// The exporter stores the current readings along with the time stamp. If the send flag is set then it sends 
// the stored data as json over USART1.
void run_exporter(exporter *e, uint8_t light_percent, uint8_t water_percent, uint8_t bat_percent);

#endif

