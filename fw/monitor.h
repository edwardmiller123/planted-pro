#ifndef MON_H
#define MON_H

#include "sensor.h"

typedef struct monitor {
	sensor * snr;
	const char * level;
	uint8_t percent;
	uint16_t min_adc_reading;
	uint16_t max_adc_reading;

} monitor;

monitor *init_monitor(sensor *s, uint16_t min_adc_reading, uint16_t max_adc_reading);

int measure_light(monitor *m);

int measure_water(monitor *m);

#define BAT_LOW_PERCENT 20

// reads the battery charge values from the sensor bufers and calculates a percentage. 
// Once the supply voltage drops below 3.4 the initial monitor values will no longer be valid
// so we instead use the internal refernce voltage to adjust the percentage stored.
int measure_battery_charge(monitor *m);

#endif