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

typedef struct monitor
{
	uint32_t sys_uptime;
	info_type currently_showing;
	light_monitor *lm;
} monitor;

void init_monitor(monitor *m, light_monitor* lm, queue* light_readings);

void run_monitor(monitor *m);

#endif