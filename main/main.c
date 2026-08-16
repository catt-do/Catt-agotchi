#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "buttons.h"
#include "display.h"
#include "pins.h"

#define TAG "Main"

static button_t left   = { .pin = PIN_BUTTON_LEFT   };
static button_t right  = { .pin = PIN_BUTTON_RIGHT  };
static button_t power  = { .pin = PIN_BUTTON_POWER  };
static button_t select = { .pin = PIN_BUTTON_SELECT };

static display_pins_t display_pins = {
    .mosi  = PIN_DISPLAY_MOSI,
    .sclk  = PIN_DISPLAY_SCLK,
    .cs    = PIN_DISPLAY_CS,
    .dc    = PIN_DISPLAY_DC,
    .reset = PIN_DISPLAY_RESET,
    .bl    = PIN_DISPLAY_BL,
};

void app_main( void )
{
    button_init( &left );
    button_init( &right );
    button_init( &power );
    button_init( &select );

    display_init( &display_pins );

    for( ;; )
    {
        if( button_is_pressed( &left) )
        {
            ESP_LOGI( TAG, "Button pressed" );
        }

        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}
