#include "actuator.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"
#include "can.h"

//angle -> 0-180

void act_servo(Servo_Act* servo_, uint8_t angle_) {

	uint8_t buf[8];

	if(servo_->status == DATTACHED) {
		attach_servo(servo_);
	}

	buf[0] = 'S'; 			// servo
	buf[1] = servo_->ID; 	// the ID
	buf[2] = 'C';			// give heads up (control)
	buf[3] = angle_;		// angle

	//UART1_Buffer(buf);

	while(CAN_Write(buf, ARDUINO_TX_INDENTIFICATOR))
			_delay_ms(50);

	servo_->angle = angle_;

}
	void setup_servo(Servo_Act* servo_, uint8_t ID_, uint8_t angle_, unsigned send) {

		servo_->ID 		= ID_;
		servo_->angle 	= angle_;

		if(send) {
			attach_servo(servo_);
			act_servo(servo_, angle_);
		} else {
			detach_servo(servo_);
		}

	}

	void attach_servo(Servo_Act* servo_) {

		uint8_t buf[8];

		buf[0] = 'S'; 			// servo
		buf[1] = servo_->ID; 	// the ID
		buf[2] = 'S';			// give heads up (status)
		buf[3] = 'A';			// attach

		//UART1_Buffer(buf);

		while(CAN_Write(buf, ARDUINO_TX_INDENTIFICATOR))
					_delay_ms(50);

		servo_->status = ATTACHED;

	}

	void detach_servo(Servo_Act* servo_) {

		uint8_t buf[8];

		buf[0] = 'S'; 			// servo
		buf[1] = servo_->ID; 	// the ID
		buf[2] = 'S';			// give heads up (status)
		buf[3] = 'D';			// deattach

		//UART1_Buffer(buf);
		while(CAN_Write(buf, ARDUINO_TX_INDENTIFICATOR))
					_delay_ms(50);

		servo_->status = DATTACHED;
	}

void act_relay(Relay_Act* relay_info) {
	//maybe user alt writer

	//relay_info.status = WAITING;

	uint8_t buf[8];

	buf[0] = 'R';
	buf[1] = relay_info.ID;
	buf[2] = relay_info.state;

	while(CAN_Write(buf, ARDUINO_TX_INDENTIFICATOR))
				_delay_ms(50);

	//altUart1Write('R');
	//altUart1Write(relay_info.ID);
	//altUart1Write(relay_info.state);

}

void act_mosfet(Mosfet_Act* mosfet_info) {

	uint8_t buf[8];

	buf[0] = 'M';
	buf[1] = mosfet_info.ID;
	buf[2] = mosfet_info.state;

	while(CAN_Write(buf, ARDUINO_TX_INDENTIFICATOR))
				_delay_ms(50);

	//altUart1Write('M');
	//altUart1Write(mosfet_info.ID);
	//altUart1Write(mosfet_info.state);

}
