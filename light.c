#include <stdint.h>

#include "adc.h"
#include "queue.h"
#include "logger.h"
#include "light.h"
#include "utils.h"
#include "fpmath.h"
#include "sensor.h"

#define BRIGHT_DIRECT_VAL 1000
#define BRIGHT_INDIRECT_VAL 1600
#define MEDIUM_VAL 2500
#define LOW_VAL 3600

const char * bright_direct = "Bright Direct";
const char * bright_indirect = "Bright Indirect";
const char * medium = "Medium";
const char * low = "Low";

// After 5 measurements have been taken we calculate the average light level and "percentage" and write it 
// back to the state struct. We use integer division as the accuracy is probably good enough for this use case.

void init_light_monitor(light_monitor *lm, sensor * s)
{
	lm->light_sensor = s;
	lm->level = NULL;
	lm->intensity_percent = UNDEFINED_PERCENTAGE;
}

int32_t set_light_level(light_monitor *lm)
{
	set_sensor_averages(lm->light_sensor);

	if (lm->light_sensor->raw_average > LOW_VAL)
	{
		lm->level = (char *)low;
	}
	else if (lm->light_sensor->raw_average > MEDIUM_VAL)
	{
		lm->level = (char *)medium;
	}
	else if (lm->light_sensor->raw_average > BRIGHT_INDIRECT_VAL)
	{
		lm->level = (char *)bright_indirect;
	}
	else
	{
		lm->level = (char *)bright_direct;
	}

	char * args_level[] = {lm->level};
	loggerf(INFO, "Light level set to &", NULL, 0, args_level, 1);

	// we want the "light" percentage and darkness gives the max adc value 
	lm->intensity_percent = 100 - lm->light_sensor->sensor_percent;

	uint32_t args_intensity[] = {lm->intensity_percent};
	loggerf(INFO, "Light intensity set to $", args_intensity, 1, NULL, 0);

	return 0;
}

int measure_light(light_monitor *ls)
{
	if (process_samples(ls->light_sensor, ls, (void *)&set_light_level) == -1)
	{
		logger(ERROR, "Failed to calculate light level");
		return -1;
	}


	if (sensor_read(ls->light_sensor, ADC1) == -1) {
		logger(ERROR, "Failed to take light sesnor reading");
		return -1;
	}
}
