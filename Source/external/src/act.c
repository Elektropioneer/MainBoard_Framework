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
		return ACT_BOARD_SUCCESS;
	else
		return ACT_BOARD_ERROR;
}

/*
 * 	Function:    static void actuator_set_status(unsigned char type, unsigned char ID, unsigned char status)
 * 	Description: set the status of a type (relay, mosfet) with an ID
 * 	Parameters:  unsigned char type   - relay or mosfet ('r', 'm')
 * 				 unsigned char ID     - the ID of the relay or mosfet
 * 				 unsigned char status - the status we set it to (ON, OFF)
 */
static void actuator_set_status(unsigned char type, unsigned char ID, unsigned char status) {

	unsigned char buffer[8];

	buffer[0] = type;				// the type (mosfet or relay - 'm', 'r')
	buffer[1] = ID;					// the ID of the mosfet/relay
	buffer[2] = 's';				// set status
	buffer[3] = status;				// the status

	while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
		_delay_ms(10);

}

/*
 * 	Function:    static unsigned char actuator_get_status(unsigned char type_status)
 * 	Description: get the status of a relay or mosfet
 * 	Parameters:  unsigned char type_status - the number in the array
 * 	Explanation: buffer[r1, r2, r3, m1, m2, m3]
 */
static unsigned char actuator_get_status(unsigned char type_status) {

	unsigned char buffer[8];

	buffer[0] = 'b';				// board
	buffer[1] = 0;					// nothing
	buffer[2] = 's';				// status

	while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
		_delay_ms(10);

	CAN_Read(buffer, DRIVER_LIFT_RX_IDENTIFICATOR);

	return buffer[type_status];

}

/*
 * 	Function:    unsigned char actuator_relay_status(unsigned char ID)
 * 	Description: gets the status of the relay
 * 	Parameters:  unsigned char ID - the ID of the relay we get status of
 */
unsigned char actuator_relay_status(unsigned char ID)  { return actuator_get_status(ID-1); }

/*
 * 	Function:    unsigned char actuator_mosfet_status(unsigned char ID)
 * 	Description: gets the status of the mosfet
 * 	Parameters:  unsigned char ID - the ID of the mosfet we get status of
 */
unsigned char actuator_mosfet_status(unsigned char ID) { return actuator_get_status(ID+2); }

/*
 * 	Function:    void actuator_relay_set(unsigned char ID, unsigned char status)
 * 	Description: set the status of the relay
 * 	Parameters:  unsigned char ID     - the ID of the relay
 * 	  			 unsigned char status -  the status of the relay (ON, OFF)
 */
void actuator_relay_set(unsigned char ID, unsigned char status)  { actuator_set_status('m', ID, status); }

/*
 * 	Function: 	 void actuator_mosfet_set(unsigned char ID, unsigned char status)
 * 	Description: set the status of the mosfet
 * 	Parameters:  unsigned char ID     - the ID of the mosfet
 * 	 			 unsigned char status - the status of the mosfet (ON, OFF)
 */
void actuator_mosfet_set(unsigned char ID, unsigned char status) { actuator_set_status('m', ID, status); }

// create specific named functions for specific ID mosfet/relay function (example: actuator_sucker(ON, OFF))




