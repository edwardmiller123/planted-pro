#ifndef PLANTMON_H
#define PLANTMON_H

#include <stdint.h>
#include <stdbool.h>

#include "monitor.h"
#include "export.h"

typedef struct plant_monitor
{
	uint32_t display_last_changed;
	uint8_t currently_showing_idx;
	monitor *lm;
	monitor *wm;
	monitor *bm;
	exporter * e;
} plant_monitor;

plant_monitor *init_plant_monitor();

void run_monitor(plant_monitor *pm);

#endif