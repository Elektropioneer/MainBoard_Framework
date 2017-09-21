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



void send_arduino(unsigned char type, unsigned char id, unsigned char data) {
	UART1_Write(type);					// the type of device we want to change
	UART1_Write((unsigned char)id);		// the id of the specific device
	UART1_Write((unsigned char)data);	// the data

	UART1_Write('X');					// end bit
}

int main()
{


	system_init();

	UART1_Init(UART1_BAUD, UART_ISR_ON);


	while(1){
		/*
		send_arduino('R', 1, 1);
		_delay_ms(1000);
		send_arduino('R', 1, 0);
		_delay_ms(1000); */

/*
				send_arduino('S', 1, 255);
				_delay_ms(1000);
				send_arduino('S', 1, 0);
				_delay_ms(1000);*/
		send_arduino('M', 3, 1);
				_delay_ms(1000);
				send_arduino('M', 3, 0);
				_delay_ms(1000);
	}

	return 0;

}//end of main()
