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
#include "usart.h"

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
static char jumper_pulled = false;


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
			while(1) {
				PORTG = ~PORTG;
				_delay_ms(1000);
			}
		}
	}

	// for testing if the timer is working
	/*if((sys_time % 1000) == 0) {

		PORTG = ~PORTG;
	}*/

	sys_time++;


}

void system_reset_system_time(void) 	{ 	sys_time = 0; 			}
void system_set_match_started(void) 	{ 	match_started = 1; 		}
uint32_t system_get_system_time(void) 	{ 	return sys_time; 		}
uint8_t system_get_match_started(void) 	{ 	return match_started; 	}


ISR(INT7_vect) {
	jumper_pulled = true;
}

static void system_setup_jumper() {

	DDRE &= ~(1 << PIN7);			// setup pin as input

	PORTE |= (1 << PIN7);			// pullup

	EICRB = (1 << ISC70);			// set on status change to triggure interrupt

	EIMSK |= (1 << INT7);			// enable interrupt

	_delay_ms(100);

	sei();							// enable global interrupts
}

static void system_wait_for_jumper() {

	while(jumper_pulled == false)		// wait for jumper_pulled t be set true
		_delay_ms(10);

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

	_delay_ms(100);

	gpio_register_pin(0, GPIO_DIRECTION_INPUT, true);

	// small delay
	_delay_ms(100);

	DDRG = 0xff;
	PORTG = 0x00;

	// waiting for jumper
	timer_init(1000);
	CAN_Init(1);
	UART1_Init(UART1_BAUD, UART_ISR_OFF);

	_delay_ms(500);

	system_setup_jumper();

	//detection_setup();
	//debug_init();


	// waiting for jumper
	//system_wait_for_jumper();
	PORTG = 0xff;


	system_reset_system_time();															// reset system time
	system_set_match_started();															// match has started!

}

