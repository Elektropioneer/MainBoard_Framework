#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gpio.h"
#include "debug.h"

//pin and the starting state of debug pins
uint8_t debug_pins[5][2] = {
		{48, OFF},					// PORTG0
		{49, OFF},					// PORTG1
		{50, OFF},					// PORTG2
		{51, OFF},					// PORTG3
		{52, OFF}					// PORTG4
};

void debug_init() {

	// total divided by one element -> number of elements (row)
	int num_of_elements = ((int) (sizeof (debug_pins) / sizeof (debug_pins)[0]));

	for(int i=0; i < num_of_elements; i++) {
		gpio_register_pin(debug_pins[i][0], GPIO_DIRECTION_OUTPUT, false);				// register pin as output and no pull up
		gpio_write_pin(debug_pins[i][0], debug_pins[i][1]);								// set the "starting" value
	}
}

void debug_switch(uint8_t pin) {
	uint8_t reverted_state = ~debug_pins[pin][1];										// creating reverted state

	memcpy(&debug_pins[pin][1], &reverted_state, 8);									// copy to the array
	gpio_write_pin(debug_pins[pin][0], reverted_state);									// write it to the pin
}

void debug_set(uint8_t pin, uint8_t state) 	{
	memcpy(&debug_pins[pin][1], &state, 8);												// copy to array
	gpio_write_pin(debug_pins[pin][0], state);											// write it to the pin
}
