#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "score_display.h"
#include "ax.h"
#include "usart.h"

static unsigned char display_send(unsigned char buffer[]) {

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
	while(UART1_Read() != DISPLAY_BOARD_SUCCESS) {
		_delay_ms(10);						// delay if not success
		error_counter++;					// error counter++

		// if error counter is above 100 -> 10ms*100 -> 1000ms -> 1s waiting
		if(error_counter > 100)
			return DISPLAY_BOARD_ERROR;			// return error
	}

	return DISPLAY_BOARD_SUCCESS;				// return success

}

void score_display_init() {

	UART1_Init(UART1_BAUD, UART_ISR_ON);

}

unsigned char update_score(unsigned char update_by) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';						// board
	buffer[1] = 's';						// id 0
	buffer[2] = 'u';						// ping
	buffer[3] = update_by;					// the value it will be updated by

	return display_send(buffer);

}

unsigned char clear_score() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';
	buffer[1] = 's';
	buffer[3] = 'c';

	return display_send(buffer);

}


