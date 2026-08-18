#include "catt.h"
#include <assert.h>
#include "stddef.h"

static uint8_t clamp( uint8_t val , uint8_t min , uint8_t max )
{
    if ( val < min ) return min;
    if ( val > max ) return max;
    return val;
}

static uint8_t decrease( uint8_t val , uint8_t decrement )
{
    if ( val < decrement ) return 0;
    return val - decrement;
}

#define STARTING_FULLNESS  50
#define STARTING_HAPPINESS 50
#define STARTING_WELLNESS  50

#define STAT_REPLENISH 20
#define STAT_DECAY      5

void catt_init( catt_t * catt )
{
    catt->fullness  = STARTING_FULLNESS;
    catt->happiness = STARTING_HAPPINESS;
    catt->wellness  = STARTING_WELLNESS;
}

void catt_stat_decrease( catt_t * catt )
{
    assert( catt != NULL );
    catt->fullness  = decrease( catt->fullness  , STAT_DECAY );
    catt->happiness = decrease( catt->happiness , STAT_DECAY );
    catt->wellness  = decrease( catt->wellness  , STAT_DECAY );
}

void catt_give_milk( catt_t * catt )
{
    assert( catt != NULL );
    catt->fullness = clamp( catt->fullness + STAT_REPLENISH , 0 , 100 );
}

void catt_give_heart( catt_t * catt )
{
    assert( catt != NULL );
    catt->happiness = clamp( catt->happiness + STAT_REPLENISH , 0 , 100 );
}

void catt_give_meds( catt_t * catt )
{
    assert( catt != NULL );
    catt->wellness = clamp( catt->wellness + STAT_REPLENISH , 0 , 100 );
}

bool catt_satisfied( catt_t * catt )
{
    assert( catt != NULL );
    if ( catt->fullness == 0 || catt->happiness == 0 || catt->wellness == 0 ) return false;
    return true;
}

bool catt_alive( catt_t * catt )
{
    assert( catt != NULL );
    if ( catt->fullness == 0 && catt->happiness == 0 && catt->wellness == 0 ) return false;
    return true;
}
