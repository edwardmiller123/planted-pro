#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

#define UNDEFINED_PERCENTAGE 101

typedef struct sensor {
	queue * readings_queue;
	uint32_t raw_average;
	uint32_t sensor_percent;
	uint32_t max_adc_reading;
	uint32_t sample_size;

} sensor;

void init_sensor(sensor *s, queue *q, uint32_t max_adc_reading, uint32_t sample_size);

// sets the average values from the already measured readings
int32_t set_sensor_averages(sensor *s);

// manually take a sesnor reading from the given ADC
int sensor_read(sensor *s, adc adc_num);

// process the readings taken so far with the given processing function if the 
// sample size has been reached
int process_samples(sensor * s, void * monitor, int (*action)(void *));

#endif