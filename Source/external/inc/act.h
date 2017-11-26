/*
 * act.h
 *
 *  Created on: Nov 25, 2017
 *      Author: silardg
 */

#ifndef SOURCE_EXTERNAL_INC_ACT_H_
#define SOURCE_EXTERNAL_INC_ACT_H_

#define ACT_BOARD_SUCCESS	1
#define ACT_BOARD_ERROR		0

#define RELAY1_ID			1
#define RELAY2_ID			2
#define RELAY3_ID			3

#define MOSFET1_ID			1
#define MOSFET2_ID 			2
#define MOSFET3_ID			3

#define ON					1
#define OFF					0

unsigned char actuator_ping(void);

#endif /* SOURCE_EXTERNAL_INC_ACT_H_ */
