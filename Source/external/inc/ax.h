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

#define ON					1
#define OFF					0

#define SERVO1_ID	4
#define SERVO2_ID 	10
#define SERVO3_ID 	2

#define all_to_zero(buffer) memset(buffer, 0, sizeof buffer);

void ax_init();
unsigned char ax_board_ping(void);
unsigned char ax_move(unsigned char ID, unsigned char position, unsigned char speed);
unsigned char ax_led(unsigned char ID, unsigned char status);
unsigned char ax_moving(unsigned char ID);
unsigned char ax_sync(unsigned char angles[3]);


#endif /* SOURCE_EXTERNAL_INC_AX_H_ */
