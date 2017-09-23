#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#define MOS_1_ON send_arduino('M', 1, 1)
#define MOS_1_OFF send_arduino('M', 1, 0);

void send_arduino(unsigned char type, unsigned char id, unsigned char data);
unsigned char get_arduino(unsigned char type, unsigned char id);


#endif /* INC_ACTUATOR_H_ */
