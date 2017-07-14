/*
 * sensors.h
 *
 *  Created on: Jul 14, 2017
 *      Author: silard_g
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#define DETECTED				1
#define NOT_DETECTED			0

// these depends on the pin config with detection pins
#define SENSOR_FRONT_LEFT		0
#define SENSOR_FRONT_RIGHT		1
#define SENSOR_FRONT_MIDDLE		2

#define SENSOR_BACK_LEFT		3
#define SENSOR_BACK_RIGHT		4
#define SENSOR_BACK_MIDDLE		5

extern uint8_t detection_pins[6][2];

void detection_setup(void);
uint8_t detection_get(uint8_t sensor_num);

#endif /* INC_SENSORS_H_ */
