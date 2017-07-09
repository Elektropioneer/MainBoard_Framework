/*
 * ax.h
 *
 *  Created on: Jul 1, 2017
 *      Author: silard_g
 */

#ifndef INC_AX_H_
#define INC_AX_H_

#define START	0xFF

#define LENGTH  0x07
#define INSTR	0x03
#define ADDRESS 0x1E

#define SPEED_H 0x02
#define SPEED_L 0x00

void AX_UART_Init(void);
void AX_SendChar(unsigned char data);
unsigned char AX_ReadChar(void);
void AX_ReadStatus(void);
void AX_StopTransmitter(void);
void AX_ReleaseTransmitter(void);
void AX_StopReceiver(void);
void AX_ReleaseReceiver(void);
void AX_SetAngle(unsigned int angle, unsigned int speed, unsigned char direction, unsigned char id);
void AX_EndlessTurn(void);
void AX_SetSpeed(unsigned int direction, unsigned int speed);

#endif /* INC_AX_H_ */
