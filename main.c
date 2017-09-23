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
#include "ax.h"
#include "system.h"
#include "sides.h"
#include "sensors.h"
#include "debug.h"
#include "can.h"
#include <avr/interrupt.h>


#define MOS_1_ON send_arduino('M', 1, 1)
#define MOS_1_OFF send_arduino('M', 1, 0);

void send_arduino(unsigned char type, unsigned char id, unsigned char data) {
	UART1_Write(type);					// the type of device we want to change
	UART1_Write((unsigned char)id);		// the id of the specific device
	UART1_Write((unsigned char)data);	// the data

	UART1_Write('X');					// end bit
}

int main()
{


	system_init();

	UART1_Init(UART1_BAUD, UART_ISR_OFF);

	_delay_ms(3000);
	while(1) {
 	send_arduino('G','M',1);
	UART1_Read();// it sends 2x the data idk why
	while(1) {
	if(UART1_Read() == 0) {
		send_arduino('M',1,1);
		break;
	}
	}
	_delay_ms(1000);
	send_arduino('G','M',1);
	UART1_Read();
	while(1) {
		if(UART1_Read() == 1) {
			send_arduino('M',1,0);
			break;
		}
		}
	_delay_ms(2000);
	}
	return 0;

}//end of main()
