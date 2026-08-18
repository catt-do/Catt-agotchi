#pragma once

#include <stdbool.h>
#include "driver/gpio.h"

typedef struct
{
    gpio_num_t pin;
}   button_t;

void button_init( button_t * button);

bool button_is_pressed( button_t * button);
