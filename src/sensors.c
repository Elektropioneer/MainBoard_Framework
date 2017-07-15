#include <avr/io.h>
#include <stdio.h>
#include "gpio.h"
#include "sensors.h"

// the pins that the sensors will be connected to, for small robot this will be only 4
uint8_t detection_pins[6][2] = {
		{0,false},	// PA0
		{1,false},	// PA1
		{2,false},	// PA2
		{3,false},	// PA3
		{4,false}, 	// PA4
		{5,false}	// PA5
};


void detection_setup(void) {

	// get the number of elements in the array 4-6
	int num_of_elements = ((int) (sizeof (detection_pins) / sizeof (detection_pins)[0]));

	for(int i=0; i < (num_of_elements); i++) {
		gpio_register_pin(detection_pins[i][0], GPIO_DIRECTION_INPUT, detection_pins[i][1]); // register the pin as input
	}

}

uint8_t detection_get(uint8_t sensor_num) {
	return gpio_read_pin(sensor_num);
}
