#include "system.h"
#include <stdint.h>
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
#include "debug.h"

/*
 * 0 PA0 		8  PB0 		16 PC0 		24 PD0 		32 PE0 		40 PF0		48 PG0
 * 1 PA1 		9  PB1 		17 PC1 		25 PD1 		33 PE1 		41 PF1		49 PG1
 * 2 PA2 		10 PB2 		18 PC2 		26 PD2 		34 PE2 		42 PF2      50 PG2
 * 3 PA3 		11 PB3 		19 PC3 		27 PD3 		35 PE3 		43 PF3      51 PG3
 * 4 PA4 		12 PB4 		20 PC4 		28 PD4 		36 PE4 		44 PF4
 * 5 PA5 		13 PB5 		21 PC5 		29 PD5 		37 PE5 		45 PF5
 * 6 PA6 		14 PB6 		22 PC6 		30 PD6 		38 PE6 		46 PF6
 * 7 PA7 		15 PB7 		23 PC7 		31 PD7 		39 PE7 		47 PF7
 */


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
			odometry_match_end();
			PORTG = 0xff;
			while(1);
		}
	}
	/*if((sys_time % 1000) == 0) {

		PORTG = ~PORTG;
	}*/

	sys_time++;


}

void system_reset_system_time(void) 	{ 	sys_time = 0; 			}
void system_set_match_started(void) 	{ 	match_started = 1; 		}
uint32_t system_get_system_time(void) 	{ 	return sys_time; 		}
uint8_t system_get_match_started(void) 	{ 	return match_started; 	}

void check_jumper(uint8_t pin) {



	//debug_set(0, ON);
}

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

	timer_init(1000);
	CAN_Init(1);
	//detection_setup();
	//debug_init();

	//check_jumper(PIN_JUMPER);

	system_reset_system_time();															// reset system time
	system_set_match_started();															// match has started!

}

