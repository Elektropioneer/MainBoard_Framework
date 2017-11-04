#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include <stdio.h>

void servo_init(unsigned int f_pwm);

void servo_set_angle_one(float angle);
void servo_set_angle_two(float angle);
void servo_set_angle_three(float angle);
void servo_set_angle_four(float angle);

/* 	ACTUATOR FUNCTIONS	*/
uint8_t _actuator_ping();
uint8_t ax_set_angle(uint8_t id, uint16_t angle);
uint8_t ax_set_speed(uint8_t id, uint16_t speed);
uint8_t ax_get_status(uint8_t id, uint8_t return_option);

uint8_t mosfet_set(uint8_t id, uint16_t status);
uint8_t mosfet_status(uint8_t id);

uint8_t relay_set(uint8_t id, uint16_t status);
uint8_t relay_status(uint8_t id);

/*	  MODULE FUNCTIONS	*/
#define MODULE_VERSION_MOSFET	0
#define MODULE_VERSION_RELAY	1

void module_init(unsigned char version);
#endif /* INC_ACTUATOR_H_ */
