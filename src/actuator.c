#include "actuator.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "usart.h"



/*
 * 	Function: 	 void send_arduino(unsigned char type, unsigned char id, unsigned char data)
 * 	Description: Send data to arduino. Type is what type of device, id of the device and the data
 * 	Parameters:	 unsigned char type - the type of device ('R', 'M', 'S')
 * 				 unsigned char id   - the id of the device 1-x
 * 				 unsigned char data	- data
 */
void send_arduino(unsigned char type, unsigned char id, unsigned char data) {

	UART1_Write(type);							// the type of device we want to change
	UART1_Write((unsigned char)id);				// the id of the specific device
	UART1_Write((unsigned char)data);			// the data

	UART1_Write('X');							// end bit
}

/*
 * 	Function: 	 unsigned char get_arduino(unsigned char type, unsigned char id)
 * 	Description: Gets status of the device
 * 	Parameters:	 unsigned char type - the type of device ('R', 'M', 'S')
 * 				 unsigned char id   - the id of the device 1-x
 */
unsigned char get_arduino(unsigned char type, unsigned char id) {

	send_arduino('G',type,(unsigned char)id);	// send the "get data protocol"
	UART1_Read();								// read (because it is sending 2x)

	return UART1_Read();						// return the read

}

/*
 * 	Function: 	 void servo_move(unsigned char id, unsigned char data);
 * 	Description: Sending the angle of the id of the servo
 * 	Parameters:  unsigned char id   - the id of the servo
 * 				 unsigned char data - the angle of the servo
 */
void servo_move(unsigned char id, unsigned char data) { send_arduino('S', id, data); }

/*
 * 	Function: 	 void ping_actuator()
 * 	Description: this will ping the actuator and it will run until ping is good
 */
void ping_actuator() {

	do {

		UART1_Write('P');
		UART1_Write(0);
		UART1_Write(0);
		UART1_Write('X');

	} while(UART1_Read() != 1);
}

/*
 *	Function: 		void servo_init(unsigned int f_pwm)
 *	Parameters: 	unsigned int f_pwm
 *	Description: 	servo init
 */
void servo_init(unsigned int f_pwm)
{
	DDRE |= ((1 << PINE3) | (1 << PINE4) | (1 << PINE5));
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
static void servo_set_duty_cycle_one(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3AH = temp >> 8;
	OCR3AL = temp & 0xFF;
}
static void servo_set_duty_cycle_two(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3BH = temp >> 8;
	OCR3BL = temp & 0xFF;
}
static void servo_set_duty_cycle_three(int16_t value)
{
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



