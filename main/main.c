#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "buttons.h"
#include "display.h"
#include "catt.h"
#include "decay_timer.h"
#include "storage.h"
#include "pins.h"

#define TAG "Main"

static button_t left   = { .pin = PIN_BUTTON_LEFT   };
static button_t right  = { .pin = PIN_BUTTON_RIGHT  };
static button_t power  = { .pin = PIN_BUTTON_POWER  };
static button_t select = { .pin = PIN_BUTTON_SELECT };

static display_pins_t display_pins =
{
    .mosi  = PIN_DISPLAY_MOSI ,
    .sclk  = PIN_DISPLAY_SCLK ,
    .cs    = PIN_DISPLAY_CS   ,
    .dc    = PIN_DISPLAY_DC   ,
    .reset = PIN_DISPLAY_RESET,
    .bl    = PIN_DISPLAY_BL   ,
};

static catt_t catt;
static catt_t saved;

static decay_timer_t decay;
static int64_t       decay_saved_at_us;

#define POLL_PERIOD_MS         20
// A real Tamagotchi loses a heart roughly every 30 minutes
#define STAT_DECAY_INTERVAL_MS ( 30 * 60 * 1000 )
// How often to remember progress through the interval, so a reboot only loses a few minutes of it
#define DECAY_SAVE_PERIOD_US   ( 5LL * 60 * 1000 * 1000 )

static const display_icon_t icon_order[] =
{
    DISPLAY_ICON_NONE ,
    DISPLAY_ICON_MILK ,
    DISPLAY_ICON_HEART,
    DISPLAY_ICON_MEDS ,
};
#define ICON_COUNT ( sizeof( icon_order ) / sizeof( icon_order[0] ) )

static void apply_icon( display_icon_t icon )
{
    switch( icon )
    {
        case DISPLAY_ICON_MILK:  catt_give_milk(  &catt ); break;
        case DISPLAY_ICON_HEART: catt_give_heart( &catt ); break;
        case DISPLAY_ICON_MEDS:  catt_give_meds(  &catt ); break;
        default: break;
    }
}

// Only write flash when something actually changed
static void save_if_changed( void )
{
    if( memcmp( &saved , &catt , sizeof( catt ) ) != 0 && storage_save( &catt ) )
    {
        saved = catt;
    }
}

static void save_decay( int64_t now_us )
{
    storage_save_decay( decay.elapsed_ms );
    decay_saved_at_us = now_us;
}

void app_main( void )
{
    button_init( &left   );
    button_init( &right  );
    button_init( &power  );
    button_init( &select );

    storage_init();

    uint32_t elapsed_ms = 0;
    if( storage_load( &catt ) )
    {
        storage_load_decay( &elapsed_ms );
    }
    else
    {
        catt_init( &catt );
    }
    saved = catt;
    decay_timer_init( &decay , STAT_DECAY_INTERVAL_MS , elapsed_ms );

    display_init( &display_pins );

    size_t selected = 0;
    int64_t last_us = esp_timer_get_time();
    decay_saved_at_us = last_us;

    for( ;; )
    {
        int64_t now_us = esp_timer_get_time();
        // measure real time: the loop takes longer than its delay once drawing is counted
        uint32_t dt_ms = ( now_us - last_us ) / 1000;
        last_us += ( int64_t )dt_ms * 1000;

        // read every button every pass so a button held across a death or restart isn't seen as a new press
        bool left_pressed   = button_was_pressed( &left   );
        bool right_pressed  = button_was_pressed( &right  );
        bool select_pressed = button_was_pressed( &select );
        bool power_pressed  = button_was_pressed( &power  );

        if( catt_alive( &catt ) )
        {
            if( left_pressed )
            {
                selected = ( selected + ICON_COUNT - 1 ) % ICON_COUNT;
            }
            if( right_pressed )
            {
                selected = ( selected + 1 ) % ICON_COUNT;
            }
            if( select_pressed )
            {
                ESP_LOGI( TAG , "Action %d" , icon_order[selected] );
                apply_icon( icon_order[selected] );
            }

            uint32_t decays = decay_timer_advance( &decay , dt_ms );
            for( uint32_t i = 0; i < decays; ++i )
            {
                catt_stat_decrease( &catt );
            }
            if( decays > 0 || now_us - decay_saved_at_us >= DECAY_SAVE_PERIOD_US )
            {
                save_decay( now_us );
            }
        }
        else if( power_pressed )
        {
            // like hatching a new egg: start over with a fresh Catt
            ESP_LOGI( TAG , "Restart" );
            catt_init( &catt );
            selected = 0;
            decay_timer_reset( &decay );
            save_decay( now_us );
        }

        save_if_changed();
        display_render( &catt , icon_order[selected] );

        vTaskDelay( pdMS_TO_TICKS( POLL_PERIOD_MS ) );
    }
}
