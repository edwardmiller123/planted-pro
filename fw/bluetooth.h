#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdbool.h>

#include "export.h"

typedef enum bt_status
{
    CONNECTED,
	DISCONNECTED
} bt_status;

// Initialises the bluetooth module interrupts on PC7 and sets the device name
int configure_bluetooth();

// Sets the bluetooth state when an interrupt arrives on exti_9_5
void bluetooth_irq_handler();

bt_status get_bluetooth_status();

// returns the bluetooth changed status and resets it to false if it has changed
bool bt_status_changed();

void poll_bluetooth(exporter *e);

#endif