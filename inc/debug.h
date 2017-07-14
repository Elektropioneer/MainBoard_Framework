/*
 * debug.h
 *
 *  Created on: Jul 14, 2017
 *      Author: silard_g
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

extern uint8_t debug_pins[5][2];

void debug_init();
void debug_switch(uint8_t pin);
void debug_set(uint8_t pin, uint8_t state);

#endif /* INC_DEBUG_H_ */
