#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include <stdint.h>

// macros for state
#define ON			1
#define OFF 		0

// macros for status
#define ATTACHED	1
#define DATTACHED	0

//macros for arduino replys
#define ERROR		'E'
#define SUCCESS		'S'

//macros for setup status
#define SEND		1
#define NSEND		0

typedef struct actuator_servo {
	uint8_t 	ID;
	uint8_t 	angle;
	uint8_t 	status;
} Servo_Act;

typedef struct actuator_relay {
	uint8_t ID;
	uint8_t state;
} Relay_Act;

typedef struct actuator_mosfet {
	uint8_t ID;
	uint8_t state;
} Mosfet_Act;

void act_servo(Servo_Act* servo_, uint8_t angle_);
void setup_servo(Servo_Act* servo_, uint8_t ID, uint8_t angle, unsigned send);
void attach_servo(Servo_Act* servo_);
void detach_servo(Servo_Act* servo_);
void act_relay(Relay_Act* relay_info);
void act_mosfet(Mosfet_Act* mosfet_info);

Servo_Act servo1;

#endif /* INC_ACTUATOR_H_ */
