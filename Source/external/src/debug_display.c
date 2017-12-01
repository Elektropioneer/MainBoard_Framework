#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "score_display.h"
#include "debug_display.h"
#include "ax.h"

/*
 * 	Function:    unsigned char debug_ready()
 * 	Description: Write out "READY" on debug display
 */
unsigned char debug_ready() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';			// display
	buffer[1] = 'd';			// debug
	buffer[2] = 'r';			// ready

	return display_send(buffer);
}

/*
 * 	Function:    unsigned char debug_setup_wait()
 * 	Description: Write out "WAIT" on debug display
 */
unsigned char debug_setup_wait() {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';			// display
	buffer[1] = 'd';			// debug
	buffer[2] = 'w';			// wait

	return display_send(buffer);
}

/*
 * Function:    unsigned char debug_position(int16_t x, int16_t y)
 * Description: Write out the current position (x,y) on the debug display
 */
unsigned char debug_position(int16_t x, int16_t y) {

	unsigned char buffer[8]; all_to_zero(buffer);

	buffer[0] = 'd';			// display
	buffer[1] = 'd';			// debug
	buffer[2] = 'p';			// position
	buffer[3] = x >> 8 ;		// x in 16bit
	buffer[4] = x & 0x00FF;
	buffer[5] = y >> 8 ;		// y in 16bit
	buffer[6] = y & 0x00FF;

	return display_send(buffer);
}


