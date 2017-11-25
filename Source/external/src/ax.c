#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "ax.h"
#include <string.h>

const unsigned char null_buffer[8] = {0,0,0,0,0,0,0,0};

#define ang_to_num(angle) ((angle * 1023) / 300)
#define num_to_ang(num)	  ((num * 300) / 1023))

/*
 * 	Function:    static unsigned char ax_send(unsigned char buffer[])
 * 	Description: Sending buffer to arduino board through UART1 interface
 * 	Parameters:  unsigned char buffer[] -> the buffer we are reading from (data)
 */
static unsigned char ax_send(unsigned char buffer[]) {

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

	unsigned int error_counter;

	for(int i=0; i < 8; i++) {
		// the logic here is that it will write if data is not 0, but if there is no data, it will send a 0
		if(null_buffer[i] != buffer[i])
			UART1_Write(buffer[i]);
		else
			UART1_Write(0);
	}

	// reading uart while it is NOT success
	while(UART1_Read() != AX_BOARD_SUCCESS) {
		_delay_ms(10);						// delay if not success
		error_counter++;					// error counter++

		// if error counter is above 100 -> 10ms*100 -> 1000ms -> 1s waiting
		if(error_counter > 100)
			return AX_BOARD_ERROR;			// return error
	}

	return AX_BOARD_SUCCESS;				// return success

}

/*
 * 	Function:    void ax_init()
 * 	Description:
 */
void ax_init() {

	UART1_Init(UART1_BAUD, UART_ISR_ON); 	// enabling uart0 for 9600baud

}

// 0 - 1023 -? 0-300


/*
 * 	Function:    unsigned char ax_board_ping()
 * 	Description: Pinging AX board
 */
unsigned char ax_board_ping(void) {

	unsigned char buffer[8];

	buffer[0] = 'b';						// board
	buffer[1] = 0;							// id 0
	buffer[2] = 'p';						// ping

	return ax_send(buffer);

}

/*
 * 	Function:    unsigned char ax_move(unsigned char ID, float position, float goal_speed)
 * 	Description: Moving ax (of ID) to position (position) with goal_speed (goal_speed)
 * 	Parameters:  unsigned char ID - the id of the ax servo
 * 				 float position   - the position for the servo to go to (0-300)
 * 				 float speed      - the speed for the servo (goal speed)
 */
unsigned char ax_move(unsigned char ID, float position, float goal_speed) {

	// converting 0-300 to 0-1023
    int16_t angle = ang_to_num(position);
    int16_t speed = ang_to_num(speed);

	unsigned char buffer[8];

	buffer[0] = 'a';							// ax
	buffer[1] = ID;								// the id of the ax
	buffer[2] = 'm';							// move
	buffer[3] = (unsigned char)(angle >> 8);	// sending 16bit value
	buffer[4] = (unsigned char)(angle & 0xFF);
	buffer[5] = (unsigned char)(speed >> 8);
	buffer[6] = (unsigned char)(speed & 0xFF);

	return ax_send(buffer);

}

/*
 * 	Function:    unsigned char ax_led(unsigned char ID, unsigned char status)
 * 	Description: control the LED on the ax servo
 * 	Parameters:  unsigned char ID     - the ID of the servo
 * 				 unsigned char status - the status (1 or 0)
 */
unsigned char ax_led(unsigned char ID, unsigned char status) {

	unsigned char buffer[8];

	buffer[0] = 'a';
	buffer[1] = ID;
	buffer[3] = 'l';
	buffer[4] = status;

	return ax_send(buffer);
}
