#include "sprite.h"
#include <assert.h>
#include <stddef.h>

void sprite_opaque_runs( const sprite_t * sprite , sprite_run_fn emit , void * ctx )
{
    assert( sprite != NULL && emit != NULL );

    for( uint16_t row = 0; row < sprite->height; ++row )
    {
        const uint16_t * line = &sprite->pixels[row * sprite->width];
        uint16_t col = 0;
        while( col < sprite->width )
        {
            // find a run of non-transparent pixels to draw in one call
            uint16_t start = col;
            while( col < sprite->width && line[col] != SPRITE_TRANSPARENT_KEY )
            {
                ++col;
            }
            uint16_t run_length = col - start;
            if( run_length > 0 )
            {
                emit( sprite->x + start , sprite->y + row , run_length , &line[start] , ctx );
            }
            ++col;  // skip the transparent pixel (or end of row)
        }
    }
}

void sprite_diff_runs( const sprite_t * from ,
                       const sprite_t * to ,
                       uint16_t         background ,
                       uint16_t *       scratch ,
                       sprite_run_fn    emit ,
                       void *           ctx )
{
    assert( from != NULL && to != NULL && scratch != NULL && emit != NULL );
    assert( from->x     == to->x     && from->y      == to->y      );
    assert( from->width == to->width && from->height == to->height );

    for( uint16_t row = 0; row < to->height; ++row )
    {
        const uint16_t * old_line = &from->pixels[row * to->width];
        const uint16_t * new_line = &to->pixels[  row * to->width];
        uint16_t col = 0;
        while( col < to->width )
        {
            if( old_line[col] == new_line[col] )
            {
                ++col;
                continue;
            }

            // extend the run through nearby differences
            uint16_t start = col;
            uint16_t end   = col;
            while( col < to->width && col - end <= SPRITE_DIFF_MERGE_GAP )
            {
                if( old_line[col] != new_line[col] ) end = col;
                ++col;
            }

            uint16_t run_length = end - start + 1;
            for( uint16_t i = 0; i < run_length; ++i )
            {
                uint16_t pixel = new_line[start + i];
                scratch[i] = ( pixel == SPRITE_TRANSPARENT_KEY ) ? background : pixel;
            }
            emit( to->x + start , to->y + row , run_length , scratch , ctx );
            col = end + 1;
        }
    }
}
