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
unsigned char ax_board_ping();


#endif /* SOURCE_EXTERNAL_INC_AX_H_ */
