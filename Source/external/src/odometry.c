#include <stdint.h>
#include "odometry.h"
#include "system.h"
#include "can.h"
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include <avr/io.h>

// current speed
static uint8_t current_speed = 0;

// struct used for odometry_position (reference from this)
volatile struct odometry_position position=
{
	.x     = 0,
	.y     = 0,
	.angle = 0,
	.state = IDLE
};

/*
 * 	Function:    void odometry_set_speed(uint8_t speed)
 * 	Description: set the goal speed
 * 	Parameters:  uint8_t speed - the speed defined in pwm (255)
 */
void odometry_set_speed(uint8_t speed)
{
	if(speed == current_speed)
		return;

	uint8_t buffer[8];
	buffer[0] = 'V';
	buffer[1] = speed;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	current_speed = speed;
}

/*
 * 	Function:    void odometry_query_position(void)
 * 	Description: get the position and state
 */
void odometry_query_position(void)
{
	uint8_t buffer[8];
	buffer[0] = 'P';
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	CAN_Read(buffer, DRIVER_RX_IDENTIFICATOR);

	position.state = buffer[0];
	position.x	   = (buffer[1] << 8) | buffer[2];
	position.y	   = (buffer[3] << 8) | buffer[4];
	position.angle = (buffer[5] << 8) | buffer[6];
}

/*
 * 	Function:    static uint8_t odometry_wait_until_done(uint8_t (*callback)(uint32_t start_time))
 * 	Description: repeat callback function while the robot is moving
 * 	Parameters:  uint8_t (*callback)(uint32_t start_time) - the callback function
 */
static uint8_t odometry_wait_until_done(uint8_t (*callback)(uint32_t start_time))
{
	uint32_t time = system_get_system_time();
	do
	{
		odometry_query_position();
		if(callback != NULL)
		{
			if(callback(time) == 1)
				return ODOMETRY_FAIL;
		}
	}while(position.state == MOVING || position.state == ROTATING);

	return ODOMETRY_SUCCESS;
}

/*
 * 	Function:    void odometry_stop(int8_t type)
 * 	Description: send stop to robot - HARD_STOP or SOFT_STOP
 * 	Parameters:  int8_t type - HARD or SOFT stop
 */
void odometry_stop(int8_t type)
{
	uint8_t buffer[8];

	do
	{
		buffer[0] = type;

		while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
			_delay_ms(10);

		odometry_query_position();
	}while(position.state == MOVING || position.state == ROTATING);
}

/*
 * 	Function:    uint8_t odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 * 	Description: send the robot to a distance, at a specific speed, while executing callback
 * 	Parameters:  int16_t distance 						  - the distance in mm (if - it will go backwards)
 * 				 uint8_t speed    						  - the goal speed
 * 				 uint8_t (*callback)(uint32_t start_time) - the callback function which will execute
 */
uint8_t odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);
	buffer[0] = 'D';
	buffer[1] = distance >> 8;
	buffer[2] = distance & 0xFF;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	return odometry_wait_until_done(callback);
}

/*
 * 	Function:    uint8_t odometry_move_to_position(struct odometry_position* position, uint8_t speed, uint8_t direction, uint8_t (*callback)(uint32_t start_time))
 * 	Description: send the robot to a specific x,y,angle position, with a specific goal speed, in a direction while executing callback
 * 	Paramters:   struct odometry_position* position       - the position the robot will go to
 * 				 uint8_t speed    				          - the goal speed
 * 				 uint8_t direction					      - the direction, forward or backward
 * 				 uint8_t (*callback)(uint32_t start_time) - the callback function
 */
uint8_t odometry_move_to_position(struct odometry_position* position, uint8_t speed, uint8_t direction, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];

	odometry_set_speed(speed);

	buffer[0] = 'G';
	buffer[1] = position->x >> 8;
	buffer[2] = position->x & 0xFF;
	buffer[3] = position->y >> 8;
	buffer[4] = position->y & 0xFF;
	buffer[5] = 0;
	buffer[6] = direction;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	return odometry_wait_until_done(callback);
}

/*
 * 	Function:    void odometry_set_position(struct odometry_position* new_position)
 * 	Description: set the robot to a current position
 * 	Parameters:  struct odometry_position* new_position - the new struct position
 */
void odometry_set_position(struct odometry_position* new_position)
{
	uint8_t buffer[8];

	buffer[0] = 'I';
	buffer[1] = new_position->x >> 8;
	buffer[2] = new_position->x & 0xFF;
	buffer[3] = new_position->y >> 8;
	buffer[4] = new_position->y & 0xFF;
	buffer[5] = new_position->angle >> 8;
	buffer[6] = new_position->angle & 0xFF;

	position.x	   = new_position->x;
	position.y	   = new_position->y;
	position.angle = new_position->angle;

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);
}

/*
 * 	Function:    uint8_t odometry_rotate_for(uint16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 * 	Description: tell the robot to rotate for a certain angle at a specific speed while doing a callback
 * 	Parameters:  uint16_t angle - the angle it will do
 * 				 uint8_t speed  - the certain speed
 * 				 uint8_t (*callback)(uint32_t start_time) -  the callback it will execute
 *
 */
uint8_t odometry_rotate_for(uint16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{

	uint8_t buffer[8];
	odometry_set_speed(speed);

	buffer[0] = 'T';
	buffer[1] = angle >> 8;
	buffer[2] = angle & 0xFF;

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	return odometry_wait_until_done(callback);

}

/*
 * 	Function:    uint8_t odometry_set_angle(uint16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 * 	Description: set the robot to  specific angle at a specific speed while doing the callback
 * 	Parameters:  uint16_t angle - the angle we set it to
 * 				 uint8_t speed  - the goal speed
 * 				 uint8_t (*callback)(uint32_t start_time) - the callback function it will execute
 */
uint8_t odometry_set_angle(uint16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);

	buffer[0] = 'A';
	buffer[1] = angle >> 8;
	buffer[2] = angle & 0xFF;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	return odometry_wait_until_done(callback);
}

uint8_t odometry_kurva(uint16_t x_pos, uint16_t y_pos, int8_t angle, uint8_t direction, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];

	odometry_set_speed(speed);

	buffer[0] = 'Q';
	buffer[1] = x_pos >> 8;
	buffer[2] = x_pos & 0xFF;
	buffer[3] = y_pos >> 8;
	buffer[4] = y_pos & 0xFF;
	buffer[5] = angle >> 8;
	buffer[6] = angle & 0xFF;
	buffer[7] = direction;

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(10);

	return odometry_wait_until_done(callback);
}

/*
 * 	Function: void odometry_match_end()
 * 	Description: match end, where it turns off the pwm
 */
void odometry_match_end()
{
	uint8_t buffer[8];

	buffer[0] = 'K';

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
			_delay_ms(10);

}

/*
 * 	Update position
 */
uint8_t getState(void)
{
	odometry_query_position();
	return position.state;
}

int16_t odometry_get_x(void)
{
	odometry_query_position();
	return position.x;
}

int16_t odometry_get_y(void)
{
	odometry_query_position();
	return position.y;
}

int16_t odometry_get_angle(void)
{
	odometry_query_position();
	return position.angle;
}
