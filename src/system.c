#include <stdint.h>
#include "system.h"
#include "gpio.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list_generic.h"
#include "can.h"

static volatile unsigned long sys_time;
static uint8_t match_started;
static void (*timer_callback)(void) = NULL;

unsigned int received = 0;

/*
 *	Function: 		void timer_register_callback(void (*callback)(void))
 *	Parameters: 	void (*callback)(void))
 *	Description: 	timer callbacks
 */
void timer_register_callback(void (*callback)(void))
{
    timer_callback = callback;
}

/*
 *	Function: 		void timer_init(unsigned int freq)
 *	Parameters: 	unsigned int freq - frequency of timer
 *	Description: 	setup timer
 */
void timer_init(unsigned int freq)
{
    TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = (double)F_CPU / (double)freq + 0.5;
	TIMSK1 = 1 << OCIE1A;

	SREG |= 0x80;
}

/*
 *	Function: 		ISR(TIMER1_COMPA_vect)
 *	Description: 	ISR interrupt for timer
 */
ISR(TIMER1_COMPA_vect)
{
    if(timer_callback != NULL)
        timer_callback();
	if(sys_time >= 90000)					// if match end
	{
		if(system_get_match_started())		// if the match has started before
		{

			while(1);
		}
	}
	sys_time++;
}


/*
 *	Function: 		void system_reset_system_time(void)
 *	Parameters: 	void
 *	Description:	reset system time
 */
void system_reset_system_time(void)
{
	sys_time = 0;
}

/*
 *	Function: 		void system_set_match_started(void)
 *	Parameters: 	void
 *	Description:	let the match begiiiiiiiiiin
 */
void system_set_match_started(void)
{
	match_started = 1;
}

/*
 *	Function: 		uint32_t system_get_system_time(void)
 *	Parameters: 	void
 *	Description:	returns current system time
 */
uint32_t system_get_system_time(void)
{
	return sys_time;
}

/*
 *	Function: 		uint8_t system_get_match_started(void)
 *	Parameters: 	void
 *	Description:	return if match has started
 */
uint8_t system_get_match_started(void)
{
	return match_started;
}

/*
 *	Function: 		uint8_t return_active_state(void)
 *	Parameters: 	void
 *	Description:	return active state
 */
/*
uint8_t return_active_state(void)
{
	return active_state;
}
*/
/*
 *	Function: 		void system_init(void)
 *	Parameters: 	void
 *	Description:	initing all the things
 */
void system_init(void)
{
	// sets debouncer
	timer_register_callback(gpio_debouncer);

	// small delay
	_delay_ms(100);

	CAN_Init(1);


	system_reset_system_time();															// reset system time
	system_set_match_started();															// match has started!

}

