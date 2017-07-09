/*
 * sensors.h
 *
 *  Created on: Jun 11, 2017
 *      Author: silard_g
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

/*
load->f0
clock->f1
enable->f2
out->f3 //input

https://github.com/Xeranor/driver_74HC165
 */

#define SENS_NUMBER_OF_CHIPS			1
#define SENS_DATA_WIDTH					SENS_NUMBER_OF_CHIPS * 8
#define SENS_PULSE_WIDTH_USEC			5
#define SENS_POLL_DELAY_MSEC			1

#define SENS_PLOAD_PIN

#endif /* INC_SENSORS_H_ */
