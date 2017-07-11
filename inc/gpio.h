/*
 * gpio.h
 *
 *  Created on: Jun 14, 2017
 *      Author: silard_g
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdint.h>
#include <stdbool.h>


#define GPIO_DEBOUNCE_COUNT         3

#define GPIO_DIRECTION_INPUT        0
#define GPIO_DIRECTION_OUTPUT       1

#define ON							1
#define OFF							0

uint8_t gpio_register_pin(uint8_t pin, uint8_t direction, bool pulled_up);
uint8_t gpio_read_pin(uint8_t pin);
uint8_t gpio_write_pin(uint8_t pin, bool value);

void    gpio_debouncer(void);


#endif /* INC_GPIO_H_ */
