#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "score_display.h"
#include "ax.h"
#include "usart.h"

int current_score = 0;

/*
 * 	Function:    static unsigned char display_send(unsigned char buffer[])
 * 	Description: send the buffer to the display
 * 	Parameters:  unsigned char buffer[] - the buffer we will send
 */
unsigned char display_send(unsigned char buffer[]) {

	unsigned char error_counter = 0;

	for(int i=0; i<8; i++) {
		UART1_Write(buffer[i]);
	}


	// reading uart while it is NOT success
	while(UART1_Read() != AX_BOARD_SUCCESS) {
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

/*
 * 	Function:    unsigned char update_score(unsigned char update_by)
 * 	Description: Update the score by update_by
 * 	Parameters:  unsigned char update_by - we will update the score by that alot
 */
unsigned char update_score(int update_by) {

	unsigned char buffer[8]; all_to_zero(buffer);

	update_by += current_score;
	current_score = update_by;

	buffer[0] = 'd';						// display
	buffer[1] = 's';						// score
	buffer[2] = 'u';						// update
	buffer[3] = update_by/100;
	buffer[4] = (update_by/10) % 10;
	buffer[5] = update_by % 10;

	return display_send(buffer);

}

/*
 * 	Function:    unsigned char clear_score()
 * 	Description: Clear the score to 000
 */
unsigned char clear_score() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';					    // display
	buffer[1] = 's';						// score
	buffer[2] = 'c';						// clear

	return display_send(buffer);

}

/*
 * 	Function:    unsigned char flash_score()
 * 	Description: Flash the display
 */
unsigned char flash_score() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';						// display
	buffer[1] = 's';						// score
	buffer[2] = 'f';						// flash

	return display_send(buffer);


}


