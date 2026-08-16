#pragma once
#include "driver/gpio.h"
#include "catt.h"
#include "st7789.h"

typedef struct
{
    gpio_num_t mosi;
    gpio_num_t sclk;
    gpio_num_t cs;
    gpio_num_t dc;
    gpio_num_t reset;
    gpio_num_t bl;
} display_pins_t;

typedef enum
{
    DISPLAY_ICON_NONE,
    DISPLAY_ICON_MILK,
    DISPLAY_ICON_MEDS,
    DISPLAY_ICON_VAPE,
} display_icon_t;

void display_init( const display_pins_t * pins );

void display_render( catt_t * catt , display_icon_t icon );
