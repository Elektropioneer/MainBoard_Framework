#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "list_generic.h"

struct gpio_input
{
    uint8_t          pin;
    bool             pulled_up;
    uint8_t          values[GPIO_DEBOUNCE_COUNT];
    uint8_t          previous_state;

    struct list_head list;
};

LIST_HEAD(gpio_inputs);

uint8_t gpio_register_pin(uint8_t pin, uint8_t direction, bool pulled_up)
{
    struct gpio_input* input;
    list_for_each_entry(input, &gpio_inputs, list)
    {
        if(input->pin == pin)
            return 1;
    }

	uint8_t port	= 1 + pin / 8;
	uint8_t pin_reg = pin % 8;
	if(direction == GPIO_DIRECTION_OUTPUT)
	{
		*(volatile uint8_t*)(0x21 + 3 *  (port - 1)) |= (1 << pin_reg);
		*(volatile uint8_t*)(0x22 + 3 *  (port - 1)) &= ~(1 << pin_reg);
        return 0;
	}
	else
		*(volatile uint8_t*)(0x21 + 3 *  (port - 1)) &= ~(1 << pin_reg);

    input = (struct gpio_input*)malloc(sizeof(struct gpio_input));
    if(input == NULL)
        return 1;

    input->pin       = pin;
    input->pulled_up = pulled_up;
    list_add(&input->list, &gpio_inputs);

    return 0;
}

uint8_t gpio_write_pin(uint8_t pin, bool value)
{
	uint8_t port = 1 + pin / 8;
	pin = pin % 8;

	uint8_t temp = *(volatile uint32_t*)(0x22 + 3 * (port - 1));
	if(value != 0)
		*(volatile uint32_t*)(0x22 + 3 * (port - 1)) = temp | (1 << pin);
	else
		*(volatile uint32_t*)(0x22 + 3 * (port - 1)) = temp & ~(1 << pin);
	return 0;
}

static uint8_t __gpio_read_pin(uint8_t pin)
{
    uint8_t port = 1 + pin / 8;
    pin = pin % 8;

	uint8_t temp = (*((volatile uint8_t*)(0x20 + 3 * (port - 1))));

    return ((temp >> pin) & 0x01);
}

uint8_t gpio_read_pin(uint8_t pin)
{
    struct gpio_input* input;
    list_for_each_entry(input, &gpio_inputs, list)
    {
        if(pin == input->pin)
        {
            uint8_t i = 0;
            while(i < GPIO_DEBOUNCE_COUNT - 1)
            {
                if(input->values[i] != input->values[i + 1])
                    return input->previous_state;
                i++;
            }

            uint8_t state = input->pulled_up == false ? (input->values[0]) : !(input->values[0]);
            input->previous_state = state;

            return state;
        }
    }

    return 1;
}
uint16_t test = 0;
void gpio_debouncer(void)
{
    static volatile count = 0;
    struct gpio_input* input;
    list_for_each_entry(input, &gpio_inputs, list)
    {
        input->values[count] = __gpio_read_pin(input->pin);
    }

    if(++count == GPIO_DEBOUNCE_COUNT)
        count = 0;

}
