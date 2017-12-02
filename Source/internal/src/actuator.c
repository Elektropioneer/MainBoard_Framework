#include "actuator.h"
#include "can.h"
#include "gpio.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t module_mosfet_status[3][3] = {
		{1, false, MOSFET_1_PIN},
		{2, false, MOSFET_2_PIN},
		{3, false, MOSFET_3_PIN}
};

uint8_t module_relay_status = 0;

/*
 *	Function: 		void servo_init(unsigned int f_pwm)
 *	Parameters: 	unsigned int f_pwm
 *	Description: 	servo init
 */
void servo_init(unsigned int f_pwm) {
	DDRE |= (1 << PINE3) | (1 << PINE4) | (1 << PINE5);
	DDRB |= (1 << PINB7);										//OCR0A 8bit

	TCNT3 = 0;
	TCNT0 = 0;

	OCR3A = 0;
	OCR3B = 0;
	OCR3C = 0;

	TCCR3A = (1 << COM3A1)  | (1 << COM3B1) | (1 << COM3B0) | (1 << COM3C1) | (1 << COM3C0) | (1 << WGM31);
	TCCR3B = (1<< CS31) | (1 << WGM32) | (1 << WGM33) ; 		// PRESKALER = 8

	//8bit timer
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << CS01) | (1 << CS00);

	ICR3   = ((double)F_CPU) / (8.0 * f_pwm) + 0.5;
}

/*
 *	Function: 		static void servo_set_duty_cycle_one-two-three(int16_t value)
 *	Parameters: 	int16_t value - the pwm value
 *	Description: 	duty cycle for servos
 *	Pin:			PE3, PE4, PE5
 */
static void servo_set_duty_cycle_one(int16_t value) {
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3AH = temp >> 8;
	OCR3AL = temp & 0xFF;
}
static void servo_set_duty_cycle_two(int16_t value) {
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3BH = temp >> 8;
	OCR3BL = temp & 0xFF;
}
static void servo_set_duty_cycle_three(int16_t value) {
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3CH = temp >> 8;
	OCR3CL = temp & 0xFF;
}

/*
 * 	Function: 	 static int16_t range_conv(float angle)
 * 	Descritpion: this function converts 0-180 to int16_t value
 */
static int16_t range_conv(float angle) {
	return ((65535 * angle) / 180) - 32768;
}

/*
 * 	Function: 	 static char check_servo_range(float angle)
 * 	Description: returns a 1 if the range is good and a 0 if not
 */
static char check_servo_range(float angle) {
	if(angle >= 0.0 && angle <= 180.0)
		return 1;
	else
		return 0;
}

/*
 * 	Function: 	void servo_set_angle_one-two-three(uint8_t angle)
 * 	Descrition: the angle goes from 0.0 - 180.0
 */
void servo_set_angle_one(float angle) {
	if(check_servo_range(angle))
		servo_set_duty_cycle_one(range_conv(angle));
}

void servo_set_angle_two(float angle) {
	if(check_servo_range(angle))
		servo_set_duty_cycle_two(range_conv(angle));
}

void servo_set_angle_three(float angle) {
	if(check_servo_range(angle))
		servo_set_duty_cycle_three(range_conv(angle));
}

// this is a 8bit register (pwm)
void servo_set_angle_four(float angle) {
	if(check_servo_range(angle))
		OCR0A = (int)((255 * angle) / 180);
}

/*
 * 	Function: 	 void module_init(unsigned char version)
 * 	Description: Init the EP_Module
 * 	Parameters:  unsigned char version -> the version we want to use
 */
void module_init(unsigned char version) {

	/* https://github.com/Elektropioneer/EP_Module */

	/*
	 * 	Version:
	 * 	0 -> mosfet module
	 * 	1 -> relay module
	 */

	if(version) {
		// relay module
		gpio_register_pin(RELAY_PIN, GPIO_DIRECTION_OUTPUT, false);		// PG0

	} else {
		// mosfet module
		gpio_register_pin(48, GPIO_DIRECTION_OUTPUT, false);			// PG0
		gpio_register_pin(49, GPIO_DIRECTION_OUTPUT, false);			// PG1
		gpio_register_pin(50, GPIO_DIRECTION_OUTPUT, false);			// PG2

	}
}

/*
 * 	Function:    void module_set_relay_status(bool status)
 * 	Description: sets the relay to the parameter status and updates the "database"
 * 	Parameters:  bool status -> the status we want to set it to
 */
void module_set_relay_status(bool status) {

	gpio_write_pin(RELAY_PIN, status);
	module_relay_status = status;

}

/*
 * 	Function:    bool module_read_relay_status()
 * 	Description: returns the status of the relay
 */
bool module_read_relay_status() { return module_relay_status; }

/*
 * 	Function:    void module_set_mosfet_status(uint8_t id, bool status)
 * 	Description: sets the mosfet by the id to the parameter status and updates the "database"
 * 	Parameters:  uint8_t id -> the id of the mosfet we want to updatebool status -> the status we want to set it to
 */
void module_set_mosfet_status(uint8_t id, bool status) {

	gpio_write_pin(module_mosfet_status[id-1][2], status);
	module_mosfet_status[id-1][1] = status;

}

/*
 * 	Function:    bool module_read_mosfet_status(uint8_t id)
 * 	Description: return the status of the id of the mosfet
 */
bool module_read_mosfet_status(uint8_t id) { return module_mosfet_status[id-1][1]; }




