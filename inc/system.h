/*
 * system.h
 *
 *  Created on: Jun 6, 2017
 *      Author: silard_g
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <avr/io.h>
#include <stdint.h>


#define JUMPER_PIN		40		//portf 0 // working

#define PWM_FREQ		50

void system_init(void);

uint8_t system_get_match_started(void);
uint32_t system_get_system_time(void);
void system_set_match_started(void);
void system_reset_system_time(void);
void check_jumper(uint8_t pin);

void flip_debug_led();

void debug_switch(uint8_t pin);
void debug_set(uint8_t pin, uint8_t state);



#endif /* INC_SYSTEM_H_ */
