#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "ax.h"
#include <string.h>

#define ang_to_num(y) ((y * 255) / 300)
#define num_to_ang(y) ((y * 300) / 255)


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

	for(int i=0; i<8; i++) {
		UART1_Write(buffer[i]);
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

/*
 * 	Function:    unsigned char ax_board_ping()
 * 	Description: Pinging AX board
 */
unsigned char ax_board_ping(void) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'b';						// board
	buffer[1] = 0;							// id 0
	buffer[2] = 'p';						// ping

	return ax_send(buffer);

}

/*
 * 	Function:    unsigned char ax_move(unsigned char ID, int position, int goal_speed)
 * 	Description: Moving ax (of ID) to position (position) with goal_speed (goal_speed)
 * 	Parameters:  unsigned char ID - the id of the ax servo
 * 				 int position   - the position for the servo to go to (0-300)
 * 				 int speed      - the speed for the servo (goal speed)
 */
unsigned char ax_move(unsigned char ID, unsigned char position, unsigned char speed) {

	// converting 0-300 to 0-255

	unsigned char _angle = (255 / (300 / position));
	unsigned char _speed = (255 / (100 / speed));

    unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'a';							// ax
	buffer[1] = ID;								// the id of the ax
	buffer[2] = 'm';							// move
	buffer[3] = _angle;
	buffer[4] = _speed;

	return ax_send(buffer);

}

/*
 * 	Function:    unsigned char ax_led(unsigned char ID, unsigned char status)
 * 	Description: control the LED on the ax servo
 * 	Parameters:  unsigned char ID     - the ID of the servo
 * 				 unsigned char status - the status (1 or 0)
 */
unsigned char ax_led(unsigned char ID, unsigned char status) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'a';
	buffer[1] = ID;
	buffer[2] = 'l';
	buffer[3] = status;

	return ax_send(buffer);
}
/*
 * 	Function:    unsigned char ax_moving(unsigned char ID)
 * 	Description: returns a 1 if the servo is moving
 * 	Parameters:  unsigned char ID - the ID of the servo
 */
unsigned char ax_moving(unsigned char ID) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'a';
	buffer[1] = ID;
	buffer[2] = 'M';

	ax_send(buffer);

	return UART1_Read();
}

/*
 * 	Function:    unsigned char ax_sync(unsigned char angles[3])
 * 	Description: Sync write to all servos
 * 	Parameters:  unsigned char angles[3] - the angles
 * 					[0] - ID 4 angle
 * 					[1] - ID 2 angle
 * 					[2] - ID 10 angle
 */
unsigned char ax_sync(unsigned char angles[3]) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'a';
	buffer[1] = 0;
	buffer[2] = 's';
	buffer[3] = (255 / (300 / angles[0]));		// id 4 angle
	buffer[4] = (255 / (300 / angles[1]));		// id 2 angle
	buffer[5] = (255 / (300 / angles[2]));		// id 10 angle

	return ax_send(buffer);
}
