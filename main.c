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

int main()
{

	system_init();

	// test this
	debug(0, ON);

	//darkside();

	return 0;

}//end of main()
