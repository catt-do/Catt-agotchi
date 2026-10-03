#pragma once
#include <stdint.h>

// Must match LIME_GREEN in scripts/png_to_rgb565.py
#define SPRITE_TRANSPARENT_KEY 0x07E0

// differing pixels this many columns apart or less are sent as one run
#define SPRITE_DIFF_MERGE_GAP 8

// A sprite is the cropped pixel box of an image that was authored at display scale.
// (x, y) is where the box's top-left corner sits on the screen.
typedef struct
{
    uint16_t         x;
    uint16_t         y;
    uint16_t         width;
    uint16_t         height;
    const uint16_t * pixels;
} sprite_t;

// Receives a horizontal run of `length` pixels to put on screen starting at (x, y)
typedef void ( *sprite_run_fn )( uint16_t x , uint16_t y , uint16_t length , const uint16_t * pixels , void * ctx );

// Emits every run of non-transparent pixels, one row at a time
void sprite_opaque_runs( const sprite_t * sprite , sprite_run_fn emit , void * ctx );

// Emits only the runs needed to turn `from` into `to` on a screen showing `background`
// behind the sprite. Both must share the same box. `scratch` must hold `to->width` pixels.
void sprite_diff_runs( const sprite_t * from ,
                       const sprite_t * to ,
                       uint16_t         background ,
                       uint16_t *       scratch ,
                       sprite_run_fn    emit ,
                       void *           ctx );
