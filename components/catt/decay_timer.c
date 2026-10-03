#include "decay_timer.h"
#include <assert.h>
#include <stddef.h>

void decay_timer_init( decay_timer_t * timer , uint32_t interval_ms , uint32_t elapsed_ms )
{
    assert( timer != NULL );
    assert( interval_ms > 0 );
    timer->interval_ms = interval_ms;
    timer->elapsed_ms  = ( elapsed_ms < interval_ms ) ? elapsed_ms : 0;
}

uint32_t decay_timer_advance( decay_timer_t * timer , uint32_t dt_ms )
{
    assert( timer != NULL );
    uint64_t total = ( uint64_t )timer->elapsed_ms + dt_ms;
    timer->elapsed_ms = total % timer->interval_ms;
    return total / timer->interval_ms;
}

void decay_timer_reset( decay_timer_t * timer )
{
    assert( timer != NULL );
    timer->elapsed_ms = 0;
}
