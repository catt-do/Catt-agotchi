#include "unity.h"
#include "catt.h"

void setUp( void )
{
    /* Nothing to setup */
}

void tearDown( void )
{
    /* Nothing to teardown */
}

void test_catt_init_sets_starting_stats( void )
{
    catt_t catt;

    catt_init( &catt );

    TEST_ASSERT_EQUAL( 50, catt.fullness );
    TEST_ASSERT_EQUAL( 50, catt.happiness );
    TEST_ASSERT_EQUAL( 50, catt.wellness );
}

void test_catt_give_milk_increases_fullness( void )
{
    catt_t catt;

    catt_init( &catt );
    catt_give_milk( &catt );

    TEST_ASSERT_EQUAL( 70, catt.fullness );
}

void test_catt_give_milk_caps_at_100( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 95;

    catt_give_milk( &catt );

    TEST_ASSERT_EQUAL( 100, catt.fullness );
}

void test_catt_stat_decrease_lowers_all_stats( void )
{
    catt_t catt;

    catt_init( &catt );
    catt_stat_decrease( &catt );

    TEST_ASSERT_EQUAL( 45, catt.fullness );
    TEST_ASSERT_EQUAL( 45, catt.happiness );
    TEST_ASSERT_EQUAL( 45, catt.wellness );
}

void test_catt_stat_decrease_never_underflows_below_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 3;

    catt_stat_decrease( &catt );

    TEST_ASSERT_EQUAL( 0, catt.fullness );
}

void test_catt_satisfied_is_false_if_any_stat_is_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 0;

    TEST_ASSERT_FALSE( catt_satisfied( &catt ) );
}

void test_catt_satisfied_is_true_if_all_stats_above_0( void )
{
    catt_t catt;

    catt_init( &catt );

    TEST_ASSERT_TRUE( catt_satisfied( &catt) );
}

void test_catt_alive_is_false_only_if_all_stats_are_0( void )
{
    catt_t catt;

    catt_init( &catt );

    catt.fullness = 0;
    catt.happiness = 0;
    catt.wellness = 0;

    TEST_ASSERT_FALSE( catt_alive( &catt ) );
}

void test_catt_alive_is_true_if_even_one_stat_is_above_0( void )
{
    catt_t catt;

    catt_init( &catt );

    catt.fullness = 0;
    catt.happiness = 0;
    catt.wellness = 1;

    TEST_ASSERT_TRUE( catt_alive( &catt ) );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_catt_init_sets_starting_stats );
    RUN_TEST( test_catt_give_milk_increases_fullness );
    RUN_TEST( test_catt_give_milk_caps_at_100 );
    RUN_TEST( test_catt_stat_decrease_lowers_all_stats );
    RUN_TEST( test_catt_stat_decrease_never_underflows_below_0 );
    RUN_TEST( test_catt_satisfied_is_false_if_any_stat_is_0 );
    RUN_TEST( test_catt_satisfied_is_true_if_all_stats_above_0 );
    RUN_TEST( test_catt_alive_is_false_only_if_all_stats_are_0 );
    RUN_TEST( test_catt_alive_is_true_if_even_one_stat_is_above_0 );

    return UNITY_END();
}
