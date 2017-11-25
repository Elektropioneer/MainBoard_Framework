#include <avr/io.h>
#include <util/delay.h>
#include "can.h"
#include "act.h"
#include <string.h>

	/*
	 * 		Protocol for actuator
	 * 		buffer[0] - ID OF TOOL (ax, relay, mosfet)
	 * 		buffer[1] - THE EXACT ID OF THE TOOL
	 * 		buffer[2] - THE FUNCTION TO DO (position, move, turn on off)
	 * 		buffer[3] - value >> 8
	 * 		buffer[4] - value & 0xFF
	 * 		buffer[5] - value2 >> 8
	 * 		buffer[6] - value2 & 0xFF
	 * 		buffer[7] - 0
	 *
	 */

/*
 * 	Function:    unsigned char actuator_ping(void)
 * 	Description: Pings the actuator board
 */
unsigned char actuator_ping(void) {

	unsigned char buffer[8];

	buffer[0] = 'b';				// board
	buffer[1] = 0;					// nothing
	buffer[2] = 'p';				// ping

	// write the data to the driver lift
	while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
		_delay_ms(10);

	// read back
	CAN_Read(buffer, DRIVER_LIFT_RX_IDENTIFICATOR);

	// if the buffer first bit is a 1 than the ping is successfull
	if(buffer[0] == 1)
		return 1;
	else
		return 0;
}
