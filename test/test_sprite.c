#include "unity.h"
#include "sprite.h"

#define K SPRITE_TRANSPARENT_KEY
#define BACKGROUND 0x1234

#define MAX_RUNS   16
#define MAX_PIXELS 32

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t length;
    uint16_t pixels[MAX_PIXELS];
} run_t;

static run_t runs[MAX_RUNS];
static int   run_count;

static void record( uint16_t x , uint16_t y , uint16_t length , const uint16_t * pixels , void * ctx )
{
    TEST_ASSERT_TRUE( run_count < MAX_RUNS );
    TEST_ASSERT_TRUE( length <= MAX_PIXELS );
    run_t * run = &runs[run_count++];
    run->x      = x;
    run->y      = y;
    run->length = length;
    for( uint16_t i = 0; i < length; ++i )
    {
        run->pixels[i] = pixels[i];
    }
}

void setUp( void )
{
    run_count = 0;
}

void tearDown( void )
{
    /* Nothing to teardown */
}

static void diff( const sprite_t * from , const sprite_t * to )
{
    uint16_t scratch[MAX_PIXELS];
    sprite_diff_runs( from , to , BACKGROUND , scratch , record , NULL );
}

// ---- sprite_opaque_runs ----

void test_opaque_runs_emits_nothing_for_a_fully_transparent_sprite( void )
{
    const uint16_t px[] = { K , K , K , K };
    sprite_t sprite = { 0 , 0 , 4 , 1 , px };

    sprite_opaque_runs( &sprite , record , NULL );

    TEST_ASSERT_EQUAL( 0 , run_count );
}

void test_opaque_runs_emits_one_run_for_a_fully_opaque_row( void )
{
    const uint16_t px[] = { 1 , 2 , 3 , 4 };
    sprite_t sprite = { 0 , 0 , 4 , 1 , px };

    sprite_opaque_runs( &sprite , record , NULL );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 0 , runs[0].x );
    TEST_ASSERT_EQUAL( 4 , runs[0].length );
    TEST_ASSERT_EQUAL_UINT16_ARRAY( px , runs[0].pixels , 4 );
}

void test_opaque_runs_splits_around_transparent_pixels( void )
{
    const uint16_t px[] = { K , 1 , 2 , K , 3 };
    sprite_t sprite = { 0 , 0 , 5 , 1 , px };

    sprite_opaque_runs( &sprite , record , NULL );

    TEST_ASSERT_EQUAL( 2 , run_count );
    TEST_ASSERT_EQUAL( 1 , runs[0].x );
    TEST_ASSERT_EQUAL( 2 , runs[0].length );
    TEST_ASSERT_EQUAL( 1 , runs[0].pixels[0] );
    TEST_ASSERT_EQUAL( 2 , runs[0].pixels[1] );
    TEST_ASSERT_EQUAL( 4 , runs[1].x );
    TEST_ASSERT_EQUAL( 1 , runs[1].length );
    TEST_ASSERT_EQUAL( 3 , runs[1].pixels[0] );
}

void test_opaque_runs_includes_a_run_that_ends_at_the_row_edge( void )
{
    const uint16_t px[] = { K , K , 7 , 8 };
    sprite_t sprite = { 0 , 0 , 4 , 1 , px };

    sprite_opaque_runs( &sprite , record , NULL );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 2 , runs[0].x );
    TEST_ASSERT_EQUAL( 2 , runs[0].length );
}

void test_opaque_runs_does_not_carry_a_run_across_rows( void )
{
    const uint16_t px[] = { 1 , 2 ,
                            3 , 4 };
    sprite_t sprite = { 0 , 0 , 2 , 2 , px };

    sprite_opaque_runs( &sprite , record , NULL );

    TEST_ASSERT_EQUAL( 2 , run_count );
    TEST_ASSERT_EQUAL( 0 , runs[0].y );
    TEST_ASSERT_EQUAL( 1 , runs[1].y );
}

void test_opaque_runs_offsets_by_the_sprite_position( void )
{
    const uint16_t px[] = { K , 5 ,
                            6 , K };
    sprite_t sprite = { 83 , 108 , 2 , 2 , px };

    sprite_opaque_runs( &sprite , record , NULL );

    TEST_ASSERT_EQUAL( 2 , run_count );
    TEST_ASSERT_EQUAL( 84  , runs[0].x );
    TEST_ASSERT_EQUAL( 108 , runs[0].y );
    TEST_ASSERT_EQUAL( 83  , runs[1].x );
    TEST_ASSERT_EQUAL( 109 , runs[1].y );
}

// ---- sprite_diff_runs ----

void test_diff_emits_nothing_for_identical_frames( void )
{
    const uint16_t px[] = { 1 , K , 3 , 4 };
    sprite_t a = { 10 , 20 , 4 , 1 , px };

    diff( &a , &a );

    TEST_ASSERT_EQUAL( 0 , run_count );
}

void test_diff_emits_a_single_changed_pixel_with_its_new_colour( void )
{
    const uint16_t from_px[] = { 1 , 2 , 3 , 4 };
    const uint16_t to_px[]   = { 1 , 9 , 3 , 4 };
    sprite_t from = { 10 , 20 , 4 , 1 , from_px };
    sprite_t to   = { 10 , 20 , 4 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1  , run_count );
    TEST_ASSERT_EQUAL( 11 , runs[0].x );
    TEST_ASSERT_EQUAL( 20 , runs[0].y );
    TEST_ASSERT_EQUAL( 1  , runs[0].length );
    TEST_ASSERT_EQUAL( 9  , runs[0].pixels[0] );
}

void test_diff_paints_the_background_where_a_pixel_became_transparent( void )
{
    const uint16_t from_px[] = { 1 , 2 };
    const uint16_t to_px[]   = { 1 , K };
    sprite_t from = { 0 , 0 , 2 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 2 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( BACKGROUND , runs[0].pixels[0] );
}

void test_diff_draws_a_pixel_that_became_opaque( void )
{
    const uint16_t from_px[] = { 1 , K };
    const uint16_t to_px[]   = { 1 , 6 };
    sprite_t from = { 0 , 0 , 2 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 2 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 6 , runs[0].pixels[0] );
}

void test_diff_ignores_pixels_transparent_in_both_frames( void )
{
    const uint16_t from_px[] = { K , 2 };
    const uint16_t to_px[]   = { K , 3 };
    sprite_t from = { 0 , 0 , 2 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 2 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 1 , runs[0].x );
}

void test_diff_merges_differences_up_to_the_merge_gap_apart( void )
{
    // columns 0 and 8 differ; 1..7 do not
    uint16_t from_px[10] = { 0 };
    uint16_t to_px[10]   = { 0 };
    for( int i = 0; i < 10; ++i )
    {
        from_px[i] = to_px[i] = 100 + i;
    }
    to_px[0] = 1;
    to_px[8] = 2;
    sprite_t from = { 0 , 0 , 10 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 10 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 0 , runs[0].x );
    TEST_ASSERT_EQUAL( 9 , runs[0].length );
    TEST_ASSERT_EQUAL_UINT16_ARRAY( to_px , runs[0].pixels , 9 );
}

void test_diff_does_not_merge_differences_farther_than_the_merge_gap( void )
{
    // columns 0 and 9 differ
    uint16_t from_px[10] = { 0 };
    uint16_t to_px[10]   = { 0 };
    for( int i = 0; i < 10; ++i )
    {
        from_px[i] = to_px[i] = 100 + i;
    }
    to_px[0] = 1;
    to_px[9] = 2;
    sprite_t from = { 0 , 0 , 10 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 10 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 2 , run_count );
    TEST_ASSERT_EQUAL( 0 , runs[0].x );
    TEST_ASSERT_EQUAL( 1 , runs[0].length );
    TEST_ASSERT_EQUAL( 9 , runs[1].x );
    TEST_ASSERT_EQUAL( 1 , runs[1].length );
}

void test_diff_fills_unchanged_pixels_inside_a_merged_run_from_the_new_frame( void )
{
    const uint16_t from_px[] = { 1 , 5 , K , 7 };
    const uint16_t to_px[]   = { 2 , 5 , K , 8 };
    sprite_t from = { 0 , 0 , 4 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 4 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 4 , runs[0].length );
    TEST_ASSERT_EQUAL( 2 , runs[0].pixels[0] );
    TEST_ASSERT_EQUAL( 5 , runs[0].pixels[1] );
    // a transparent pixel in the gap is the background, never the chroma key
    TEST_ASSERT_EQUAL( BACKGROUND , runs[0].pixels[2] );
    TEST_ASSERT_EQUAL( 8 , runs[0].pixels[3] );
}

void test_diff_includes_a_difference_in_the_last_column( void )
{
    const uint16_t from_px[] = { 1 , 2 , 3 };
    const uint16_t to_px[]   = { 1 , 2 , 9 };
    sprite_t from = { 0 , 0 , 3 , 1 , from_px };
    sprite_t to   = { 0 , 0 , 3 , 1 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 1 , run_count );
    TEST_ASSERT_EQUAL( 2 , runs[0].x );
    TEST_ASSERT_EQUAL( 9 , runs[0].pixels[0] );
}

void test_diff_emits_separate_runs_for_separate_rows( void )
{
    const uint16_t from_px[] = { 1 , 2 ,
                                 3 , 4 };
    const uint16_t to_px[]   = { 9 , 2 ,
                                 3 , 8 };
    sprite_t from = { 5 , 6 , 2 , 2 , from_px };
    sprite_t to   = { 5 , 6 , 2 , 2 , to_px   };

    diff( &from , &to );

    TEST_ASSERT_EQUAL( 2 , run_count );
    TEST_ASSERT_EQUAL( 5 , runs[0].x );
    TEST_ASSERT_EQUAL( 6 , runs[0].y );
    TEST_ASSERT_EQUAL( 6 , runs[1].x );
    TEST_ASSERT_EQUAL( 7 , runs[1].y );
}

void test_diff_applied_to_a_screen_reproduces_the_new_frame( void )
{
    // paint `from`, apply the diff, and expect `to` on a screen of background
    const uint16_t from_px[] = { K , 1 , 2 , K , K , 3 , 4 , 5 , K , K , K , 6 };
    const uint16_t to_px[]   = { 7 , 1 , K , K , 8 , 3 , K , 5 , 9 , K , K , K };
    sprite_t from = { 2 , 1 , 6 , 2 , from_px };
    sprite_t to   = { 2 , 1 , 6 , 2 , to_px   };

    uint16_t screen[10 * 5];
    uint16_t expected[10 * 5];
    for( int i = 0; i < 10 * 5; ++i )
    {
        screen[i] = expected[i] = BACKGROUND;
    }

    for( int row = 0; row < 2; ++row )
    {
        for( int col = 0; col < 6; ++col )
        {
            uint16_t f = from_px[row * 6 + col];
            uint16_t t = to_px[row * 6 + col];
            screen[( 1 + row ) * 10 + 2 + col]   = ( f == K ) ? BACKGROUND : f;
            expected[( 1 + row ) * 10 + 2 + col] = ( t == K ) ? BACKGROUND : t;
        }
    }

    diff( &from , &to );
    for( int r = 0; r < run_count; ++r )
    {
        for( int i = 0; i < runs[r].length; ++i )
        {
            screen[runs[r].y * 10 + runs[r].x + i] = runs[r].pixels[i];
        }
    }

    TEST_ASSERT_EQUAL_UINT16_ARRAY( expected , screen , 10 * 5 );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_opaque_runs_emits_nothing_for_a_fully_transparent_sprite );
    RUN_TEST( test_opaque_runs_emits_one_run_for_a_fully_opaque_row         );
    RUN_TEST( test_opaque_runs_splits_around_transparent_pixels             );
    RUN_TEST( test_opaque_runs_includes_a_run_that_ends_at_the_row_edge     );
    RUN_TEST( test_opaque_runs_does_not_carry_a_run_across_rows             );
    RUN_TEST( test_opaque_runs_offsets_by_the_sprite_position               );

    RUN_TEST( test_diff_emits_nothing_for_identical_frames                          );
    RUN_TEST( test_diff_emits_a_single_changed_pixel_with_its_new_colour            );
    RUN_TEST( test_diff_paints_the_background_where_a_pixel_became_transparent      );
    RUN_TEST( test_diff_draws_a_pixel_that_became_opaque                            );
    RUN_TEST( test_diff_ignores_pixels_transparent_in_both_frames                   );
    RUN_TEST( test_diff_merges_differences_up_to_the_merge_gap_apart                );
    RUN_TEST( test_diff_does_not_merge_differences_farther_than_the_merge_gap       );
    RUN_TEST( test_diff_fills_unchanged_pixels_inside_a_merged_run_from_the_new_frame );
    RUN_TEST( test_diff_includes_a_difference_in_the_last_column                    );
    RUN_TEST( test_diff_emits_separate_runs_for_separate_rows                       );
    RUN_TEST( test_diff_applied_to_a_screen_reproduces_the_new_frame                );

    return UNITY_END();
}
