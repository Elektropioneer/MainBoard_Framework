#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

// controlling mosfets with defines
#define MOS_1_ON send_arduino('M', 1, 1)
#define MOS_1_OFF send_arduino('M', 1, 0)
#define MOS_2_ON send_arduino('M', 2, 1)
#define MOS_2_OFF send_arduino('M', 2, 0)
#define MOS_3_ON send_arduino('M', 2, 1)
#define MOS_3_OFF send_arduino('M', 2, 0)

// controlling relays with defines
#define REL_1_ON send_arduino('R',1,1)
#define REL_1_OFF send_arduino('R',1,0)
#define REL_2_ON send_arduino('R',2,1)
#define REL_2_OFF send_arduino('R',2,0)
#define REL_3_ON send_arduino('R',3,1)
#define REL_3_OFF send_arduino('R',3,0)

// controlling servos with defines (it is converting 0-180 range to 0-255 range)
#define SER_1(angle) servo_move(1, ((180 * angle) / 255) )			// 0 - 180 angle
#define SER_2(angle) servo_move(2, ((180 * angle) / 255) )			// 0 - 180 angle
#define SER_3(angle) servo_move(3, ((180 * angle) / 255) )			// 0 - 180 angle
#define SER_4(angle) servo_move(4, ((180 * angle) / 255) )			// 0 - 180 angle
#define SER_5(angle) servo_move(5, ((180 * angle) / 255) )			// 0 - 180 angle

void send_arduino(unsigned char type, unsigned char id, unsigned char data);
unsigned char get_arduino(unsigned char type, unsigned char id);

void servo_move(unsigned char id, unsigned char data);

#endif /* INC_ACTUATOR_H_ */
