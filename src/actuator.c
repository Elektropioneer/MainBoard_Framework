#include "actuator.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "usart.h"

/*
 * 	Function: 	 void send_arduino(unsigned char type, unsigned char id, unsigned char data)
 * 	Description: Send data to arduino. Type is what type of device, id of the device and the data
 * 	Parameters:	 unsigned char type - the type of device ('R', 'M', 'S')
 * 				 unsigned char id   - the id of the device 1-x
 * 				 unsigned char data	- data
 */
void send_arduino(unsigned char type, unsigned char id, unsigned char data) {
	UART1_Write(type);							// the type of device we want to change
	UART1_Write((unsigned char)id);				// the id of the specific device
	UART1_Write((unsigned char)data);			// the data

	UART1_Write('X');							// end bit
}

/*
 * 	Function: 	 unsigned char get_arduino(unsigned char type, unsigned char id)
 * 	Description: Gets status of the device
 * 	Parameters:	 unsigned char type - the type of device ('R', 'M', 'S')
 * 				 unsigned char id   - the id of the device 1-x
 */
unsigned char get_arduino(unsigned char type, unsigned char id) {
	send_arduino('G',type,(unsigned char)id);	// send the "get data protocol"
	UART1_Read();								// read (because it is sending 2x)

	return UART1_Read();						// return the read

}
