//
// Created by nikita on 23.05.24.
//

#include "driver/gpio.h"

static void configure_internal_led(int pin_number)
{
    gpio_reset_pin(pin_number);
    gpio_set_direction(pin_number, GPIO_MODE_OUTPUT);
}

void internal_led_on(int pin_number)
{
    configure_internal_led(pin_number);
    gpio_set_level(pin_number, 0);
}

void internal_led_off(int pin_number)
{
    configure_internal_led(pin_number);
    gpio_set_level(pin_number, 1);
}