#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <stdbool.h>

#define GPIO_DEBOUNCE_COUNT         2


#define GPIO_DIRECTION_INPUT        0
#define GPIO_DIRECTION_OUTPUT       1

uint8_t gpio_register_pin(uint8_t pin, uint8_t direction, bool pulled_up);
uint8_t gpio_read_pin(uint8_t pin);
uint8_t gpio_write_pin(uint8_t pin, bool value);

void    gpio_debouncer(void);

#endif
