#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

void servo_init(unsigned int f_pwm);

void servo_set_angle_one(float angle);
void servo_set_angle_two(float angle);
void servo_set_angle_three(float angle);
void servo_set_angle_four(float angle);

#endif /* INC_ACTUATOR_H_ */
