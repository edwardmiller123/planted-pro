#ifndef MON_H
#define MON_H

#include "sensor.h"

typedef struct monitor {
	sensor * snr;
	char * level;
	uint8_t percent;

} monitor;

monitor * init_monitor(sensor * s);

int measure_light(monitor *m);

int measure_water(monitor *m);

#endif