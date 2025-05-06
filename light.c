#include <stdint.h>

#include "adc.h"
#include "queue.h"
#include "logger.h"
#include "light.h"
#include "utils.h"

#define BRIGHT_DIRECT_VAL 1000
#define BRIGHT_INDIRECT_VAL 1600
#define MEDIUM_VAL 2500
#define LOW_VAL 3600

// max adc output using a 82k resistor in the potential divider
#define ADC_MAX_OUTPUT 4095

#define SAMPLE_SIZE 5

const char * bright_direct = "Bright Direct";
const char * bright_indirect = "Bright Indirect";
const char * medium = "Medium";
const char * low = "Low";

// After 5 measurements have been taken we calculate the average light level and "percentage" and write it 
// back to the state struct. We use integer division as the accuracy is probably good enough for this use case.

void init_light_monitor(light_monitor *lm, queue *q)
{
	reset_queue(q);
	lm->readings_queue = q;
	lm->level = NULL;
	lm->percentage = UNDEFINED_PERCENTAGE;
}

int32_t set_light_level(light_monitor *lm)
{
	uint32_t total_val = 0;
	int32_t reading;
	for (int i = 0; i < SAMPLE_SIZE; i++)
	{
		reading = fifo_get(lm->readings_queue);
		if (reading == -1)
		{
			logger(ERROR, "failed to fetch reading from queue");
			return -1;
		}
		total_val += (uint32_t)reading;
	}

	uint32_t average = total_val / SAMPLE_SIZE;
	lm->raw_average = average;

	if (average > LOW_VAL)
	{
		lm->level = (char *)low;
	}
	else if (average > MEDIUM_VAL)
	{
		lm->level = (char *)medium;
	}
	else if (average > BRIGHT_INDIRECT_VAL)
	{
		lm->level = (char *)bright_indirect;
	}
	else
	{
		lm->level = (char *)bright_direct;
	}

	char * args[] = {lm->level};
	loggerf(INFO, "Light level set to &", NULL, 0, args, 1);

	return 0;
}

void set_percentage(light_monitor *lm) {
	uint32_t percentage = (lm->raw_average / (ADC_MAX_OUTPUT)) * 100;
	// we want the "light" percentage and darkness gives the max adc value 
	lm->percentage = 100 - percentage;
}

int measure_light(light_monitor *lm)
{
	if (lm->readings_queue->size == SAMPLE_SIZE)
	{
		if (set_light_level(lm) == -1)
		{
			logger(ERROR, "Failed to calculate light level");
			return -1;
		}

		set_percentage(lm);

		reset_queue(lm->readings_queue);
	}

	int32_t reading = adc1_manual_conversion();
	if (reading == -1)
	{
		logger(ERROR, "Failed to read adc conversion");
		return -1;
	}

	if (fifo_add(lm->readings_queue, (uint32_t)reading) == -1)
	{
		logger(ERROR, "Failed to add light reading");
		return -1;
	}
	return 0;
}
