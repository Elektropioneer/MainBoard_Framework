#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gpio.h"
#include "debug.h"

//pin and the starting state of debug pins
uint8_t debug_pins[5] = {48,49,50,51,52};

void debug_init() {

	// total divided by one element -> number of elements (row)
	int num_of_elements = ((int) (sizeof (debug_pins) / sizeof (debug_pins)[0]));

	for(int i=0; i < num_of_elements; i++) {
		gpio_register_pin(debug_pins[i], GPIO_DIRECTION_OUTPUT, false);				// register pin as output and no pull up
		gpio_write_pin(debug_pins[i], OFF);								// set the "starting" value
	}
}
