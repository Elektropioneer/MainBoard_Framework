#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

#include <stdint.h>		// for uint8_t

#define BIG_ROBOT		// if it is defined we are using center sensors too
#define DEBUG_LED

#define SENSOR_FR		1
#define SENSOR_FL		3
#define SENSOR_BR		4
#define SENSOR_BL		6

#define SENSOR_FR_PIN	0
#define SENSOR_FL_PIN	1
#define SENSOR_BR_PIN	2
#define SENSOR_BL_PIN	3

// it will return 1 when it reads the trig value
#define SENSOR_FR_TRIG	1
#define SENSOR_FL_TRIG	1
#define SENSOR_BR_TRIG	1
#define SENSOR_BL_TRIG	1

#ifdef BIG_ROBOT

#define SENSOR_FC		2
#define SENSOR_BC		5

#define SENSOR_FC_PIN	4
#define SENSOR_BC_PIN	5

#define SENSOR_FC_TRIG	1
#define SENSOR_BC_TRIG	1

uint8_t sensor_fc(); uint8_t sensor_bc();

#endif

uint8_t sensor_fr(); uint8_t sensor_fl();
uint8_t sensor_br(); uint8_t sensor_bl();

uint8_t sensor_all_front(); uint8_t sensor_all_back();

#endif /* INC_SENSORS_H_ */
