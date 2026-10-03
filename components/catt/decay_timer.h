#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t interval_ms;
    uint32_t elapsed_ms;
} decay_timer_t;

// `elapsed_ms` is where to resume from (e.g. loaded from storage); a value that isn't
// below the interval can't be real, so it starts over from 0
void decay_timer_init(
    decay_timer_t * timer ,
    uint32_t interval_ms ,
    uint32_t elapsed_ms
);

// Returns how many whole intervals passed. The remainder carries over, so a slow
// loop never loses time.
uint32_t decay_timer_advance( decay_timer_t * timer , uint32_t dt_ms );

void decay_timer_reset( decay_timer_t * timer );
