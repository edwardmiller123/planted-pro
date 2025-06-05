
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "fpmath.h"
#include "logger.h"
#include "adc.h"
#include "sensor.h"
#include "heap.h"

sensor * init_sensor(uint32_t min_adc_reading, uint32_t max_adc_reading, uint32_t sample_size, adc adc_num)
{
	sensor * s = malloc(sizeof(sensor));
	if (s == NULL) {
		logger(ERROR, "Failed to allocate memory for sensor");
		return NULL;
	}

	queue *readings = malloc(sizeof(queue));
	if (s == NULL) {
		logger(ERROR, "Failed to allocate memory for sensor readings queue");
		return NULL;
	}

	reset_queue(readings);
	s->readings_queue = readings;

	s->sensor_percent = UNDEFINED_PERCENTAGE;
	s->min_adc_reading = min_adc_reading;
	s->max_adc_reading = max_adc_reading;
	s->sample_size = sample_size;
	s->adc_num = adc_num;
	return s;
}

void set_percent(sensor *s)
{
	uint32_t value_range = s->max_adc_reading - s->min_adc_reading;
	uint32_t adjusted_val = s->raw_average - s->min_adc_reading;
	uint32_t percentage = fp_percentage(adjusted_val, value_range);
	s->sensor_percent = percentage;
}

int sensor_calculate_average(sensor *s)
{
	uint32_t total_val = 0;
	int32_t reading;
	for (int i = 0; i < s->sample_size; i++)
	{
		reading = fifo_get(s->readings_queue);
		if (reading == -1)
		{
			logger(ERROR, "failed to fetch reading from queue");
			return -1;
		}
		total_val += (uint32_t)reading;
	}

	uint32_t average = fp_divide(total_val, s->sample_size);
	s->raw_average = average;

	uint32_t args_average[] = {average};
	loggerf(DEBUG, "Set average sensor ADC value to $", args_average, 1, NULL, 0);

	set_percent(s);

	uint32_t args_percent[] = {s->sensor_percent};
	loggerf(INFO, "Sensor percent set to $", args_percent, 1, NULL, 0);

	return 0;
}

int sensor_read_adc(sensor *s)
{
	int32_t reading = adc_manual_conversion(s->adc_num);
	if (reading == -1)
	{
		logger(ERROR, "Failed to read adc conversion");
		return -1;
	}

	if (fifo_add(s->readings_queue, (uint32_t)reading) == -1)
	{
		logger(ERROR, "Failed to add light reading");
		return -1;
	}
	return 0;
}