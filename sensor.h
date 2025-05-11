#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#include "adc.h"
#include "queue.h"

#define UNDEFINED_PERCENTAGE 101

// generic sensor
typedef struct sensor {
	queue * readings_queue;
	uint32_t raw_average;
	uint32_t sensor_percent;
	uint32_t max_adc_reading;
	uint32_t sample_size;
	adc adc_num;

} sensor;

typedef struct monitor {
	sensor * snr;
	char * level;
	uint32_t percent;

} monitor;

void init_monitor(monitor *m, sensor * s);

void init_sensor(sensor *s, queue *q, uint32_t max_adc_reading, uint32_t sample_size, adc adc_num);

// sets the average values from the already measured readings
int sensor_calculate_average(sensor *s);

// manually take a sensor reading from the configured ADC
int sensor_read_adc(sensor *s);

// process the readings taken so far with the given processing function if the 
// sample size has been reached
int sensor_process_samples(sensor *s, monitor *m, int (*action)(struct monitor *));

#endif