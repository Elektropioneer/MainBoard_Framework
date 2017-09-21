/*
  	   .    _  .
       |\_|/__/|
       / / \/ \  \
      /__|O||O|__ \
     |/_ \_/\_/ _\ |
     | | (____) | ||
     \/\___/\__/  //
     (_/         ||
      |          ||
      |          ||\
       \        //_/
        \______//
       __ || __||
      (____(____)

 	 Electropioneer Framework for AT90CAN128 based board, developed by Silard Gal 2017.

 	 https://github.com/Elektropioneer
 	 http://elektropioneer.co.rs/

 	 For more info visit the README

 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"
#include "actuator.h"
#include "gpio.h"
#include "ax.h"
#include "system.h"
#include "sides.h"
#include "sensors.h"
#include "debug.h"
#include "can.h"
#include <avr/interrupt.h>


int main()
{


	system_init();

	/* Test uart stuff */
	/*
	uart1_init(BAUD_CALC(9600));				// TODO test with 9600

	sei();

	while(1) {

		uart1_puts("We are sending shit");

		char buffer[25];

		uart1_gets(buffer, 25);

		uart1_puts("We got: ");

		uart1_putstr(buffer);

		_delay_ms(1000);
	}

	*/
	/* End of uart stuff */

	// add uart logging the current position every second first through uart, after through bluetooth.
	// make a python app that will log all of it and maybe do some drawing aswell and a dashboard for easier debug

	//darkside();


	return 0;

}//end of main()
