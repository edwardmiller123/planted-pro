#ifndef LIGHTMON_H
#define LIGHTMON_H

#include <stdint.h>

#include "sensor.h"

typedef struct light_monitor {
	sensor * light_sensor;
	char * level;
	uint32_t intensity_percent;

} light_monitor;

void init_light_monitor(light_monitor *lm, sensor * s);

int measure_light(light_monitor *lm);

#endif