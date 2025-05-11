#ifndef MON_H
#define MON_H

#include <stdint.h>

#include "light.h"
#include "queue.h"

typedef enum info_type
{
	LIGHT,
	MOISTURE,
} info_type;

typedef struct plant_monitor
{
	uint32_t sys_uptime;
	info_type currently_showing;
	monitor *lm;
	monitor *wm;
} plant_monitor;

void init_plant_monitor(plant_monitor *pm, monitor *lm, monitor *wm, sensor * light_sensor, sensor * water_sensor, queue *light_readings, queue *water_readings);

void run_monitor(plant_monitor *pm);

#endif