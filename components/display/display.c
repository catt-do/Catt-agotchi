#include "display.h"
#include <stddef.h>
#include <assert.h>

static TFT_t dev;

//#define BAR_WIDTH
//#define BAR_HEIGHT

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320

#define BABY_PINK rgb565( 0xFD , 0xBD , 0xE4 )
#define FUSCHIA   rgb565( 0xFF , 0x00 , 0xFF )
#define VIOLET    rgb565( 0x7F , 0x00 , 0xFF )
#define LAVENDER  rgb565( 0xDF , 0xC5 , 0xFE )

static const uint16_t backgroundColor = LAVENDER;
//static const uint16_t textColor       = VIOLET;
//static const uint16_t statusbarColor  = FUSCHIA;
//static const uint16_t fillerColor     = BABY_PINK;

void display_init( const display_pins_t * pins )
{
    assert( pins != NULL );

    spi_master_init(
        &dev,
        pins->mosi,
        pins->sclk,
        pins->cs,
        pins->dc,
        pins->reset,
        pins->bl
    );

    lcdInit( &dev , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 );
}

void display_render( catt_t * catt , display_icon_t icon )
{
    lcdFillScreen( &dev , backgroundColor );
    // ...
}

static void draw_icons( display_icon_t highlighted )
{
    //...
}

static void draw_catt( catt_t * catt )
{
    //...
}

static void draw_statusbar()
{
    //...
}

static void draw_stats()
{
    //...
}

static void draw_dead_catt()
{
    //...
}
