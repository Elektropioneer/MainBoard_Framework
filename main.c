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
#include <string.h>
#include "usart.h"
#include "actuator.h"
#include "gpio.h"
#include "system.h"
#include "sides.h"
#include "sensors.h"
#include "debug.h"
#include "can.h"
#include <avr/interrupt.h>

int main()
{

	system_init();

	_delay_ms(3000);

	while(1) {
		if(sensor_all_back()) {
			PORTG = 0x01;
		} else{
			PORTG = 0x00;
		}
	}
	return 0;

}//end of main()
