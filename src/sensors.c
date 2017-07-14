#include <avr/io.h>
#include <stdio.h>
#include "gpio.h"
#include "sensors.h"

uint8_t detection_pins[6][2] = {
		{0,false},
		{1,false},
		{2,false},
		{3,false},
		{4,false},
		{5,false}
};


void detection_setup(void) {

	int num_of_elements = ((int) (sizeof (detection_pins) / sizeof (detection_pins)[0]));

	for(int i=0; i < (num_of_elements); i++) {
		gpio_register_pin(detection_pins[i][0], GPIO_DIRECTION_INPUT, detection_pins[i][1]);
	}

}

uint8_t detection_get(uint8_t sensor_num) {
	return gpio_read_pin(sensor_num);
}
