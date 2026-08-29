#include "display.h"
#include <stddef.h>
#include <assert.h>

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

static const uint16_t textColor       =    VIOLET;
static const uint16_t statusbarColor  =   FUSCHIA;
static const uint16_t backgroundColor =  LAVENDER;
static const uint16_t fillerColor     = BABY_PINK;

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
    lcdFillScreen(   &dev ,                                       backgroundColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,              fillerColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,              fillerColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii ,                textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii ,                textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii ,                textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii ,                textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii ,                textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii ,                textColor );
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );

}

static void draw_icons( display_icon_t highlighted )
{
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
}

static void draw_catt( catt_t * catt )
{
    if ( catt_satisfied( catt ) )
    {
        while ( true )
        {
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
        }
    }
    if ( catt_alive( catt ) )
    {
        while ( true )
        {
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
        }
    }
    else
    {
            spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
    }
}

static void draw_statusbar()
{
    lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d ,           statusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
    lcdDrawFillRect( &dev , a , b , c ,  d , otherstatusbarColor );
}

static void draw_stats()
{
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
    lcdDrawString(   &dev ,  &fx , x ,  y , ascii , textColor );
}

static void draw_dead_catt()
{
    spi_master_write_byte( spi_device_handle_t SPIHandle , const uint8_t * Data , size_t DataLength );
}
