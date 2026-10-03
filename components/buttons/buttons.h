#pragma once

#include <stdbool.h>
#include "driver/gpio.h"

typedef struct
{
    gpio_num_t pin;
    bool       was_down;
}   button_t;

void button_init(        button_t * button );

bool button_is_pressed(  button_t * button );

bool button_was_pressed( button_t * button );
