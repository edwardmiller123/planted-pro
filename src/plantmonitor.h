#ifndef PLANTMON_H
#define PLANTMON_H

#include <stdint.h>

#include "monitor.h"
#include "export.h"

typedef enum info_type
{
	LIGHT,
	MOISTURE,
} info_type;

typedef struct plant_monitor
{
	uint32_t display_change_interval;
	info_type currently_showing;
	monitor *lm;
	monitor *wm;
	exporter * e;
} plant_monitor;

plant_monitor * init_plant_monitor();

void run_monitor(plant_monitor *pm);

#endif