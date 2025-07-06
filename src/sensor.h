#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#include "adc.h"
#include "ringbuf.h"

#define UNDEFINED_PERCENTAGE 101

// generic sensor
typedef struct sensor {
	ring_buffer * readings_buffer;
	uint32_t raw_average;
	uint8_t sensor_percent;
	uint32_t min_adc_reading;
	uint32_t max_adc_reading;
	uint32_t sample_size;
	adc adc_num;

} sensor;

sensor * init_sensor(uint32_t min_adc_reading, uint32_t max_adc_reading, uint32_t sample_size, adc adc_num);

// sets the average values from the already measured readings
int sensor_calculate_average(sensor *s);

// manually take a sensor reading from the configured ADC
int sensor_read_adc(sensor *s);

#endif