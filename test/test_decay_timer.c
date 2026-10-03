#include "unity.h"
#include "decay_timer.h"

void setUp( void )
{
    /* Nothing to setup */
}

void tearDown( void )
{
    /* Nothing to teardown */
}

void test_decay_timer_init_resumes_from_elapsed( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 400 );

    TEST_ASSERT_EQUAL( 1000 , timer.interval_ms );
    TEST_ASSERT_EQUAL( 400  , timer.elapsed_ms  );
}

void test_decay_timer_init_discards_elapsed_that_is_not_below_interval( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 1000 );
    TEST_ASSERT_EQUAL( 0 , timer.elapsed_ms );

    decay_timer_init( &timer , 1000 , 123456 );
    TEST_ASSERT_EQUAL( 0 , timer.elapsed_ms );
}

void test_decay_timer_advance_returns_0_before_interval( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 0 );

    TEST_ASSERT_EQUAL( 0   , decay_timer_advance( &timer , 999 ) );
    TEST_ASSERT_EQUAL( 999 , timer.elapsed_ms );
}

void test_decay_timer_advance_returns_1_at_interval_and_starts_over( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 0 );

    TEST_ASSERT_EQUAL( 1 , decay_timer_advance( &timer , 1000 ) );
    TEST_ASSERT_EQUAL( 0 , timer.elapsed_ms );
}

void test_decay_timer_advance_carries_remainder_over( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 900 );

    TEST_ASSERT_EQUAL( 1   , decay_timer_advance( &timer , 150 ) );
    TEST_ASSERT_EQUAL( 50  , timer.elapsed_ms );
    TEST_ASSERT_EQUAL( 1   , decay_timer_advance( &timer , 950 ) );
    TEST_ASSERT_EQUAL( 0   , timer.elapsed_ms );
}

void test_decay_timer_advance_counts_every_missed_interval( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 0 );

    TEST_ASSERT_EQUAL( 3   , decay_timer_advance( &timer , 3500 ) );
    TEST_ASSERT_EQUAL( 500 , timer.elapsed_ms );
}

void test_decay_timer_advance_does_not_overflow( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 4000000000u , 3999999999u );

    TEST_ASSERT_EQUAL( 1 , decay_timer_advance( &timer , 3000000000u ) );
}

void test_decay_timer_reset_clears_elapsed( void )
{
    decay_timer_t timer;

    decay_timer_init( &timer , 1000 , 700 );
    decay_timer_reset( &timer );

    TEST_ASSERT_EQUAL( 0 , timer.elapsed_ms );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_decay_timer_init_resumes_from_elapsed                           );
    RUN_TEST( test_decay_timer_init_discards_elapsed_that_is_not_below_interval    );
    RUN_TEST( test_decay_timer_advance_returns_0_before_interval                   );
    RUN_TEST( test_decay_timer_advance_returns_1_at_interval_and_starts_over       );
    RUN_TEST( test_decay_timer_advance_carries_remainder_over                      );
    RUN_TEST( test_decay_timer_advance_counts_every_missed_interval                );
    RUN_TEST( test_decay_timer_advance_does_not_overflow                           );
    RUN_TEST( test_decay_timer_reset_clears_elapsed                                );

    return UNITY_END();
}
