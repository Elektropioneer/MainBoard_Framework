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

/*
 * 0 PA0 8  PB0 16 PC0 24 PD0 32 PE0 40 PF0
 * 1 PA1 9  PB1 17 PC1 25 PD1 33 PE1 41 PF1
 * 2 PA2 10 PB2 18 PC2 26 PD2 34 PE2 42 PF2
 * 3 PA3 11 PB3 19 PC3 27 PD3 35 PE3 43 PF3
 * 4 PA4 12 PB4 20 PC4 28 PD4 36 PE4 44 PF4
 * 5 PA5 13 PB5 21 PC5 29 PD5 37 PE5 45 PF5
 * 6 PA6 14 PB6 22 PC6 30 PD6 38 PE6 46 PF6
 * 7 PA7 15 PB7 23 PC7 31 PD7 39 PE7 47 PF7
 */

#define GPIO_DEBOUNCE_COUNT         3

#define GPIO_DIRECTION_INPUT        0
#define GPIO_DIRECTION_OUTPUT       1

uint8_t gpio_register_pin(uint8_t pin, uint8_t direction, bool pulled_up);
uint8_t gpio_read_pin(uint8_t pin);
uint8_t gpio_write_pin(uint8_t pin, bool value);

void    gpio_debouncer(void);

#endif /* INC_GPIO_H_ */
