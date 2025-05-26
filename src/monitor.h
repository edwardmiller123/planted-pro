#ifndef MON_H
#define MON_H

#include "sensor.h"

typedef struct monitor {
	sensor * snr;
	char * level;
	uint32_t percent;

} monitor;

void init_monitor(monitor *m, sensor * s);

int measure_light(monitor *m);

int measure_water(monitor *m);

#endif