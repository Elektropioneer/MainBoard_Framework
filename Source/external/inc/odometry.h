#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include <stdint.h>
#include <stdbool.h>

#define HARD_STOP					'S'
#define SOFT_STOP					's'

#define ODOMETRY_SUCCESS			0
#define ODOMETRY_FAIL				1
#define ODOMETRY_STUCK				2
#define ODOMETRY_CALLBACK_RETURN	3

#define LOW_SPEED					50
#define NORMAL_SPEED				150
#define HIGH_SPEED					210

#define FORWARD						1
#define BACKWARD					-1

#define COLLISION					0
#define TACTIC_ONE					1

#define BACKUP_DISTANCE 			150

struct odometry_position
{
	int16_t x;
	int16_t y;
	int16_t angle;
	int8_t  state;
};

enum odometry_states
{
	IDLE = 'I',
	MOVING = 'M',
	ROTATING = 'R',
	STUCK = 'S',
	ERROR = 'E'
};


// callback za odometriju vraca sledece vrednosti:
// 0- nista se ne desava, ostani u odometrijskoj funkciji
// 1- zelis da ispadnes iz funkcije, stop se realizuje ili u callbacku ili posle ispada, vraca ODOMETRY_FAIL
// 2- zelis da ispadnes iz funkcije, stop se realizuje kako hoces, vraca ODOMETRY_CALLBACK_RETURN
uint8_t     odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time));
uint8_t     odometry_move_to_position(struct odometry_position* position, uint8_t speed, uint8_t direction, char (*callback)(uint32_t start_time));
void	    odometry_set_position(struct odometry_position* new_position);
uint8_t     odometry_rotate_for(uint16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time));
uint8_t     odometry_set_angle(uint16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time));
uint8_t     odometry_get_status(void);
void        odometry_stop(int8_t type);
void        odometry_query_position(void);
//uint8_t 	odometry_kurva(uint16_t x_pos, uint16_t y_pos, int8_t angle, uint8_t direction, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
void 		odometry_match_end();

int16_t     odometry_get_x(void);
int16_t     odometry_get_y(void);
int16_t     odometry_get_angle(void);




#endif /* ODOMETRY_H_ */
