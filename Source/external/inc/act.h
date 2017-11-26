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

unsigned char actuator_relay_status(unsigned char ID);
unsigned char actuator_mosfet_status(unsigned char ID);
void actuator_relay_set(unsigned char ID, unsigned char status);
void actuator_mosfet_set(unsigned char ID, unsigned char status);

#endif /* SOURCE_EXTERNAL_INC_ACT_H_ */
