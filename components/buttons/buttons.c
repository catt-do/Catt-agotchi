#include "buttons.h"

void button_init( button_t * button )
{
    gpio_config_t config =
    {
        .pin_bit_mask = 1ULL << button->pin   ,
        .mode         = GPIO_MODE_INPUT       ,
        .pull_up_en   = GPIO_PULLUP_ENABLE    ,
        .pull_down_en = GPIO_PULLDOWN_DISABLE ,
        .intr_type    = GPIO_INTR_DISABLE     ,
    };
    gpio_config( &config );
}

bool button_is_pressed( button_t * button )
{
    return gpio_get_level( button->pin ) == 0;
}