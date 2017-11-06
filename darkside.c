#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "gpio.h"
#include "actuator.h"
#include "sensors.h"

// if this is defined it will execute the coordinates for the "first desk" if commented it will move on to the "second desk"
#define first_desk

// must be the same number as the same number of gotoFields, if not only some of it will execute
#define TACTIC_ONE_POSITION_COUNT	2

// variables for keeping position count, odometry return status and active state
uint8_t current_position = 0, next_position = 0, odometry_status, active_state = TACTIC_ONE, backjump_status = 0;

#ifdef first_desk
// first desk coordinates
gotoFields TACTIC_ONE_POSITION[TACTIC_ONE_POSITION_COUNT] = {
		{{450,0,0}, NORMAL_SPEED, FORWARD, sensor_all_front},
		{{0,0,0}, NORMAL_SPEED, BACKWARD, sensor_all_back}
};
#else
// second desk coordinates
gotoFields TACTIC_ONE_POSITION[TACTIC_ONE_POSITION_COUNT] = {
		{{450,0,0}, NORMAL_SPEED, FORWARD, sensor_all_front},
	    {{0,0,0}, NORMAL_SPEED, BACKWARD, sensor_all_back}
};
#endif

/*
 * 	Function: 	 static void wait_while_detection_options(uint8_t jump_enabled)
 * 	Description: checking sensors and if jump_enabled is 1 then it jumps back
 */
static void wait_while_detection_options(uint8_t jump_enabled) {

	_delay_ms(200);
	while(TACTIC_ONE_POSITION[current_position].callback(0) == 1) {
		if(backjump_status == 0 && jump_enabled == 1) {
			if(TACTIC_ONE_POSITION[current_position].direction == FORWARD) {
				odometry_move_straight(-(BACKUP_DISTANCE), NORMAL_SPEED, sensor_all_back);
			} else {
				odometry_move_straight(BACKUP_DISTANCE, NORMAL_SPEED, sensor_all_front);
			}
		}
		_delay_ms(10);
	}
	next_position = current_position;
	active_state = TACTIC_ONE;

}

static void wait_while_detection(void) 			{ wait_while_detection_options(0); }	// with no jump back when detected
static void wait_while_detection_withjump(void) { wait_while_detection_options(1); }	// with jump back when detected

void darkside(void) {


	// setting the starting position
	struct odometry_position startingPosition; startingPosition.x = 0; startingPosition.y = 0; startingPosition.angle = 0;

	// sending the starting position to odometry
	odometry_set_position(&startingPosition);

	while(1) {
		switch(active_state) {
		case COLLISION:		// COLLISION ! DON'T FORGET BREAK !
			/* if(current_position == 0) {
				wait_while_detection_tactic_one();
				break;
			} */

			wait_while_detection_withjump();
			break;
		case STUCK:			// STUCK
			_delay_ms(1000);
			active_state = TACTIC_ONE;
			next_position = current_position;

			break;
		case TACTIC_ONE:	// TACTIC ONE
			for(current_position = next_position; current_position < TACTIC_ONE_POSITION_COUNT; current_position++) {		// go through the position counts

				// send the gotoField and receive status
				odometry_status = odometry_move_to_position(&TACTIC_ONE_POSITION[current_position].point, TACTIC_ONE_POSITION[current_position].speed, TACTIC_ONE_POSITION[current_position].direction, TACTIC_ONE_POSITION[current_position].callback);

				// if odometry fails set state to collision, which is mostly triggured by stop
				if(odometry_status == ODOMETRY_FAIL)
				{
					active_state = COLLISION;
					break;
				}

				if(current_position == 0) {
					_delay_ms(2000);
				}
				// last position
				else if(current_position == (TACTIC_ONE_POSITION_COUNT - 1))
				{
					while(1);
				}
			}//end of for
		}//end of switch
	}//end of while


}//end of darkside
