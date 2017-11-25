/*
 * ax.h
 *
 *  Created on: Nov 23, 2017
 *      Author: silardg
 */

#ifndef SOURCE_EXTERNAL_INC_AX_H_
#define SOURCE_EXTERNAL_INC_AX_H_

#define AX_BOARD_SUCCESS	1
#define AX_BOARD_ERROR		0

#define SERVO1_ID	4
#define SERVO2_ID 	10
#define SERVO3_ID 	2

void ax_init();
unsigned char ax_board_ping(void);
unsigned char ax_move(unsigned char ID, float position, float goal_speed);
unsigned char ax_led(unsigned char ID, unsigned char status);


#endif /* SOURCE_EXTERNAL_INC_AX_H_ */
