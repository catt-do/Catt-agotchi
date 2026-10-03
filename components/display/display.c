#include "display.h"
#include <stddef.h>
#include <assert.h>
#include "esp_timer.h"
#include "happy_catt.h"

#include "../catt/catt.h"
#include "../st7789/st7789.h"

static TFT_t dev;

//#define BAR_WIDTH
//#define BAR_HEIGHT

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

#define VIOLET    rgb565( 0x7F , 0x00 , 0xFF )
#define FUSCHIA   rgb565( 0xFF , 0x00 , 0xFF )
#define LAVENDER  rgb565( 0xDF , 0xC5 , 0xFE )
#define BABY_PINK rgb565( 0xFD , 0xBD , 0xE4 )

//static const uint16_t textColor       =    VIOLET;
//static const uint16_t statusbarColor  =   FUSCHIA;
static const uint16_t backgroundColor =  LAVENDER;
//static const uint16_t fillerColor     = BABY_PINK;

#define IDLE_FRAME_PERIOD_US ( 500 * 1000 )

typedef struct
{
    const sprite_t * frames;
    size_t           count;
} animation_t;

// TODO: sad (4 frames) and R.I.P. (1 frame) art doesn't exist yet, so they borrow the happy frames
static const animation_t animations[] =
{
    [CATT_SATISFIED  ] = { happy_catt_frames , HAPPY_CATT_FRAME_COUNT },
    [CATT_UNSATISFIED] = { happy_catt_frames , HAPPY_CATT_FRAME_COUNT },
    [CATT_DEAD       ] = { happy_catt_frames , HAPPY_CATT_FRAME_COUNT },
};

static bool         drawn;
static catt_state_t drawn_state;
static size_t       idle_frame;
static int64_t      idle_frame_since_us;

static void draw_run( uint16_t x , uint16_t y , uint16_t length , const uint16_t * pixels , void * ctx )
{
    lcdDrawMultiPixels( &dev , x , y , length , ( uint16_t * )pixels );
}

static void draw_sprite( const sprite_t * sprite )
{
    sprite_opaque_runs( sprite , draw_run , NULL );
}

// Draws only the pixels of `to` that differ from `from`. Whatever lies under the sprite must be the background.
static void draw_sprite_diff( const sprite_t * from , const sprite_t * to )
{
    uint16_t scratch[SCREEN_WIDTH];
    sprite_diff_runs( from , to , backgroundColor , scratch , draw_run , NULL );
}

void display_init( const display_pins_t * pins )
{
    assert( pins != NULL );

    spi_master_init(
        &dev       ,
        pins->mosi ,
        pins->sclk ,
        pins->cs   ,
        pins->dc   ,
        pins->reset,
        pins->bl
    );

    lcdInit( &dev , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 );
}

void display_render( catt_t * catt , display_icon_t icon )
{
    int64_t now = esp_timer_get_time();
    catt_state_t state = catt_state( catt );

    if( !drawn || state != drawn_state )
    {
        // new state: the whole cat is different, so start over
        lcdFillScreen( &dev , backgroundColor );
        idle_frame = 0;
        draw_sprite( &animations[state].frames[idle_frame] );
        idle_frame_since_us = now;
        drawn_state = state;
        drawn = true;
        return;
    }

    const animation_t * animation = &animations[state];
    if( animation->count > 1 && now - idle_frame_since_us >= IDLE_FRAME_PERIOD_US )
    {
        size_t next = ( idle_frame + 1 ) % animation->count;
        draw_sprite_diff( &animation->frames[idle_frame] , &animation->frames[next] );
        idle_frame = next;
        idle_frame_since_us = now;
    }
}

static void draw_icons( display_icon_t highlighted )
{
//    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
//    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
//    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
}

static void draw_statusbar()
{
    //lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    //lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    //lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    //lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    //lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    //lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
}

static void draw_stats()
{
    //lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    //lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    //lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    //lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    //lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    //lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
}
