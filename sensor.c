
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "fpmath.h"
#include "logger.h"
#include "adc.h"
#include "sensor.h"

void init_sensor(sensor *s, queue *q, uint32_t max_adc_reading, uint32_t sample_size)
{
	reset_queue(q);
	s->readings_queue = q;
	s->sensor_percent = UNDEFINED_PERCENTAGE;
	s->max_adc_reading = max_adc_reading;
	s->sample_size = sample_size;
}

void set_percent(sensor *s)
{
	uint32_t percentage = fp_percentage(s->raw_average, s->max_adc_reading);
	s->sensor_percent = percentage;
}

int32_t set_sensor_averages(sensor *s)
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

int sensor_read(sensor *s, adc adc_num)
{
	int32_t reading = adc_manual_conversion(adc_num);
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

int process_samples(sensor *s, void *monitor, int (*action)(void *))
{
	if (s->readings_queue->size == s->sample_size)
	{
		if (action(monitor) == -1)
		{
			logger(ERROR, "Failed to process adc samples");
			return -1;
		}

		reset_queue(s->readings_queue);
	}
	return 0;
}