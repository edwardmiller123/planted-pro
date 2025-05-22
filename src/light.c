#include <stdint.h>

#include "adc.h"
#include "logger.h"
#include "light.h"
#include "utils.h"
#include "sensor.h"

#define BRIGHT_DIRECT_VAL 1000
#define BRIGHT_INDIRECT_VAL 1600
#define MEDIUM_VAL 2500
#define LOW_VAL 3600

const char *light_bright_direct = "Bright Direct";
const char *light_bright_indirect = "Bright Indirect";
const char *light_medium = "Medium";
const char *light_low = "Low";

int set_light_level(monitor *lm)
{
	if (sensor_calculate_average(lm->snr) == -1)
	{
		logger(ERROR, "Failed to get average light reading");
		return -1;
	};

	if (lm->snr->raw_average > LOW_VAL)
	{
		lm->level = (char *)light_low;
	}
	else if (lm->snr->raw_average > MEDIUM_VAL)
	{
		lm->level = (char *)light_medium;
	}
	else if (lm->snr->raw_average > BRIGHT_INDIRECT_VAL)
	{
		lm->level = (char *)light_bright_indirect;
	}
	else
	{
		lm->level = (char *)light_bright_direct;
	}

	char *args_level[] = {lm->level};
	loggerf(INFO, "Light level set to &", NULL, 0, args_level, 1);

	// we want the "light" percentage and darkness gives the max adc value
	lm->percent = 100 - lm->snr->sensor_percent;

	uint32_t args_intensity[] = {lm->percent};
	loggerf(INFO, "Light intensity set to $", args_intensity, 1, NULL, 0);

	return 0;
}

int measure_light(monitor *lm)
{
	if (sensor_process_samples(lm->snr, lm, &set_light_level) == -1)
	{
		logger(ERROR, "Failed to calculate light level");
		return -1;
	}

	if (sensor_read_adc(lm->snr) == -1)
	{
		logger(ERROR, "Failed to take light sensor reading");
		return -1;
	}

	return 0;
}
