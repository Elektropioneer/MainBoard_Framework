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
#include "can.h"
#include <avr/interrupt.h>
#include "ax.h"
#include "score_display.h"


int main()
{

	system_init();

	unsigned char data[3] = {150, 150, 150};

	ax_sync(data);

	return 0;

}//end of main()
