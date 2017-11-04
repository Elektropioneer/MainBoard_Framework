#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include <stdio.h>
#include "gpio.h"

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

#define RELAY_PIN				48
#define MOSFET_1_PIN			48
#define MOSFET_2_PIN			49
#define MOSFET_3_PIN			50

void module_init(unsigned char version);
void module_set_relay_status(bool status);
bool module_read_relay_status();
void module_set_mosfet_status(uint8_t id, bool status);
bool module_read_mosfet_status(uint8_t id);


#endif /* INC_ACTUATOR_H_ */
