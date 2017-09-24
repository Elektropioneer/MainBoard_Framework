#include "sensors.h"
#include "gpio.h"
#include "odometry.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*
 * 	Function: 	 All front sensors individually
 * 	Description: Returns a 1 when detected
 */
char sensor_fr() {

	if(gpio_read_pin(SENSOR_FR_PIN) == SENSOR_FR_TRIG) {
		odometry_stop(HARD_STOP);
		return 1;
	}

	return 0;
}

char sensor_fl() {

	if(gpio_read_pin(SENSOR_FL_PIN) == SENSOR_FL_TRIG) {
		odometry_stop(HARD_STOP);
		return 1;
	}

	return 0;
}

char sensor_fc() {
	#ifdef BIG_ROBOT
	if(gpio_read_pin(SENSOR_FC_PIN) == SENSOR_FC_TRIG) {
		odometry_stop(HARD_STOP);
		return 1;
	}
	#endif

	return 0;
}

/*************************************************************/

/*
 * 	Function: 	 All back sensors individually
 * 	Description: Returns a 1 when detected
 */
char sensor_br() {

	if(gpio_read_pin(SENSOR_BR_PIN) == SENSOR_BR_TRIG) {
		odometry_stop(HARD_STOP);
		return 1;
	}

	return 0;
}

char sensor_bl() {

	if(gpio_read_pin(SENSOR_BL_PIN) == SENSOR_BL_TRIG) {
		odometry_stop(HARD_STOP);
		return 1;
	}

	return 0;
}


char sensor_bc() {
	#ifdef BIG_ROBOT
	if(gpio_read_pin(SENSOR_BC_PIN) == SENSOR_BC_TRIG) {
		odometry_stop(HARD_STOP);
		return 1;
	}
	#endif

	return 0;
}

/*************************************************************/

/*
 * 	Combination functions
 */

char sensor_all_front() {
	#ifdef BIG_ROBOT
		if(sensor_fr() || sensor_fl() || sensor_fc()) {
			return 1;
		}
	#else
		if(sensor_fr() || sensor_fl()) {
			return 1;
		}
	#endif

	return 0;
}

char sensor_all_back() {
	#ifdef BIG_ROBOT
		if(sensor_br() || sensor_bl() || sensor_bc()) {
			return 1;
		}
	#else
		if(sensor_br() || sensor_bl()) {
			return 1;
		}
	#endif

	return 0;
}

/*************************************************************/







