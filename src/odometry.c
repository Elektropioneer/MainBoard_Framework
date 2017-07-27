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
 * 	void odometry_set_speed(uint8_t speed)
 *
 * 	Send odometry board the max speed
 */
void odometry_set_speed(uint8_t speed)
{
	if(speed == current_speed)
		return;

	uint8_t buffer[8];
	buffer[0] = 'V';
	buffer[1] = speed;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	current_speed = speed;
}

/*
 * 	void odometry_query_position(void)
 *
 * 	Get the current position. Update it to position
 */
void odometry_query_position(void)
{
	uint8_t buffer[8];
	buffer[0] = 'P';
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	CAN_Read(buffer, DRIVER_RX_IDENTIFICATOR);

	position.state = buffer[0];
	position.x	   = (buffer[1] << 8) | buffer[2];
	position.y	   = (buffer[3] << 8) | buffer[4];
	position.angle = (buffer[5] << 8) | buffer[6];
}

/*
 * 	static uint8_t odometry_wait_until_done(uint8_t (*callback)(uint32_t start_time))
 *
 * 	Callback function, execute function while odometry is moving
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
 * 	void odometry_stop(int8_t type)
 *
 * 	Send stop to odometry
 */
void odometry_stop(int8_t type)
{
	uint8_t buffer[8];

	do
	{
		buffer[0] = type;

		while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
			_delay_ms(50);

		odometry_query_position();
	}while(position.state == MOVING || position.state == ROTATING);
}

/*
 * 	uint8_t odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 *
 * 	Send move straight command to odometry
 */
uint8_t odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);
	buffer[0] = 'D';
	buffer[1] = distance >> 8;
	buffer[2] = distance & 0xFF;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	return odometry_wait_until_done(callback);
}

/*
 * 	uint8_t odometry_move_to_position(struct odometry_position* position, uint8_t speed, uint8_t direction, uint8_t (*callback)(uint32_t start_time))
 *
 * 	Send move to position function to odometry
 */
uint8_t odometry_move_to_position(struct odometry_position* position, uint8_t speed, uint8_t direction, char (*callback)(uint32_t start_time))
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
		_delay_ms(50);

	return odometry_wait_until_done(callback);
}

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
		_delay_ms(50);
}

/*
 * 	uint8_t odometry_rotate_for(uint16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 *
 * 	Send rotate for command to odometry board
 */
uint8_t odometry_rotate_for(uint16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{

	uint8_t buffer[8];
	odometry_set_speed(speed);

	buffer[0] = 'T';
	buffer[1] = angle >> 8;
	buffer[2] = angle & 0xFF;

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	return odometry_wait_until_done(callback);

}

/*
 * 	uint8_t odometry_set_angle(uint16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 *
 * 	Set angle for odometry
 */
uint8_t odometry_set_angle(uint16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);

	buffer[0] = 'A';
	buffer[1] = angle >> 8;
	buffer[2] = angle & 0xFF;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

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
		_delay_ms(50);

	return odometry_wait_until_done(callback);
}

void odometry_match_end(void)
{
	uint8_t buffer[8];

	buffer[0] = 'K';

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
			_delay_ms(50);

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
