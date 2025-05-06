#ifndef LIGHTMON_H
#define LIGHTMON_H

#include <stdint.h>

#include "queue.h"

#define UNDEFINED_PERCENTAGE 101

typedef struct light_monitor {
	queue * readings_queue;
	uint32_t raw_average;
	char * level;
	uint32_t percentage;

} light_monitor;

void init_light_monitor(light_monitor *lm, queue *q);

int measure_light(light_monitor *lm);

#endif