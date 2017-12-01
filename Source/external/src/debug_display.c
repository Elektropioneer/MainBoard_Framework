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

/*
 * 	Function: unsigned char debug_status(float odometry_bat, float glavna_bat, unsigned char side_name, unsigned char tactic_name)
 * 	Description: create the status on the display
 * 	Parameters:  float odometry_bat        - the calculated voltage for odometry
 * 				 float glavna_bat          - the calculated voltage for glavna
 * 				 unsigned char side_name   - the name of the side we are on
 * 				 unsigned char tactic_name - the tactic name
 */
unsigned char debug_status(float odometry_bat, float glavna_bat, unsigned char side_name, unsigned char tactic_name) {

	unsigned char buffer[8]; all_to_zero(buffer);

	unsigned char bat_od = (unsigned char)(odometry_bat * 10);
	unsigned char bat_gl = (unsigned char)(glavna_bat   * 10);

	buffer[0] = 'd';			// display
	buffer[1] = 'd';			// debug
	buffer[2] = 'i';			// info
	buffer[3] = bat_od;			// battery voltage (odometry)
	buffer[4] = bat_gl;			// battery voltage (glavna)
	buffer[5] = side_name;		// side name
	buffer[6] = tactic_name;	// tactic name

	return display_send(buffer);
}


