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

    TEST_ASSERT_EQUAL( 50 , catt.fullness  );
    TEST_ASSERT_EQUAL( 50 , catt.happiness );
    TEST_ASSERT_EQUAL( 50 , catt.wellness  );
}

void test_catt_give_milk_increases_fullness( void )
{
    catt_t catt;

    catt_init( &catt );
    catt_give_milk( &catt );

    TEST_ASSERT_EQUAL( 70 , catt.fullness );
}

void test_catt_give_milk_caps_at_100( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 95;

    catt_give_milk( &catt );

    TEST_ASSERT_EQUAL( 100 , catt.fullness );
}

void test_catt_stat_decrease_lowers_all_stats( void )
{
    catt_t catt;

    catt_init( &catt );
    catt_stat_decrease( &catt );

    TEST_ASSERT_EQUAL( 45 , catt.fullness  );
    TEST_ASSERT_EQUAL( 45 , catt.happiness );
    TEST_ASSERT_EQUAL( 45 , catt.wellness  );
}

void test_catt_stat_decrease_never_underflows_below_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 3;

    catt_stat_decrease( &catt );

    TEST_ASSERT_EQUAL( 0 , catt.fullness );
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

    catt.fullness  = 0;
    catt.happiness = 0;
    catt.wellness  = 0;

    TEST_ASSERT_FALSE( catt_alive( &catt ) );
}

void test_catt_alive_is_true_if_even_one_stat_is_above_0( void )
{
    catt_t catt;

    catt_init( &catt );

    catt.fullness  = 0;
    catt.happiness = 0;
    catt.wellness  = 1;

    TEST_ASSERT_TRUE( catt_alive( &catt ) );
}

void test_catt_state_is_satisfied_if_all_stats_above_0( void )
{
    catt_t catt;

    catt_init( &catt );

    TEST_ASSERT_EQUAL( CATT_SATISFIED , catt_state( &catt ) );
}

void test_catt_state_is_unsatisfied_if_some_but_not_all_stats_are_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.happiness = 0;

    TEST_ASSERT_EQUAL( CATT_UNSATISFIED , catt_state( &catt ) );
}

void test_catt_state_is_dead_if_all_stats_are_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness  = 0;
    catt.happiness = 0;
    catt.wellness  = 0;

    TEST_ASSERT_EQUAL( CATT_DEAD , catt_state( &catt ) );
}

void test_catt_valid_is_true_for_starting_stats( void )
{
    catt_t catt;

    catt_init( &catt );

    TEST_ASSERT_TRUE( catt_valid( &catt ) );
}

void test_catt_valid_is_true_at_the_max_of_100( void )
{
    catt_t catt = { .fullness = 100 , .happiness = 100 , .wellness = 100 };

    TEST_ASSERT_TRUE( catt_valid( &catt ) );
}

void test_catt_valid_is_false_if_any_stat_is_above_100( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.wellness = 101;

    TEST_ASSERT_FALSE( catt_valid( &catt ) );
}

void test_catt_give_heart_increases_happiness( void )
{
    catt_t catt;

    catt_init( &catt );
    catt_give_heart( &catt );

    TEST_ASSERT_EQUAL( 70 , catt.happiness );
    TEST_ASSERT_EQUAL( 50 , catt.fullness  );
    TEST_ASSERT_EQUAL( 50 , catt.wellness  );
}

void test_catt_give_meds_increases_wellness( void )
{
    catt_t catt;

    catt_init( &catt );
    catt_give_meds( &catt );

    TEST_ASSERT_EQUAL( 70 , catt.wellness  );
    TEST_ASSERT_EQUAL( 50 , catt.fullness  );
    TEST_ASSERT_EQUAL( 50 , catt.happiness );
}

void test_catt_give_heart_and_meds_cap_at_100( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.happiness = 95;
    catt.wellness  = 100;

    catt_give_heart( &catt );
    catt_give_meds(  &catt );

    TEST_ASSERT_EQUAL( 100 , catt.happiness );
    TEST_ASSERT_EQUAL( 100 , catt.wellness  );
}

void test_catt_stat_decrease_from_exactly_the_decay_amount_reaches_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness  = 5;
    catt.happiness = 4;
    catt.wellness  = 6;

    catt_stat_decrease( &catt );

    TEST_ASSERT_EQUAL( 0 , catt.fullness  );
    TEST_ASSERT_EQUAL( 0 , catt.happiness );
    TEST_ASSERT_EQUAL( 1 , catt.wellness  );
}

void test_catt_dies_after_ten_decays_with_no_care( void )
{
    catt_t catt;

    catt_init( &catt );

    for( int i = 0; i < 9; ++i )
    {
        catt_stat_decrease( &catt );
    }
    TEST_ASSERT_EQUAL( CATT_SATISFIED , catt_state( &catt ) );

    catt_stat_decrease( &catt );
    TEST_ASSERT_EQUAL( CATT_DEAD , catt_state( &catt ) );
}

void test_catt_state_is_unsatisfied_when_only_one_stat_is_left_above_0( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness  = 0;
    catt.happiness = 0;

    TEST_ASSERT_EQUAL( CATT_UNSATISFIED , catt_state( &catt ) );
}

void test_catt_state_is_unsatisfied_for_each_single_empty_stat( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 0;
    TEST_ASSERT_EQUAL( CATT_UNSATISFIED , catt_state( &catt ) );

    catt_init( &catt );
    catt.happiness = 0;
    TEST_ASSERT_EQUAL( CATT_UNSATISFIED , catt_state( &catt ) );

    catt_init( &catt );
    catt.wellness = 0;
    TEST_ASSERT_EQUAL( CATT_UNSATISFIED , catt_state( &catt ) );
}

void test_catt_state_is_satisfied_again_once_the_empty_stat_is_replenished( void )
{
    catt_t catt;

    catt_init( &catt );
    catt.fullness = 0;
    catt_give_milk( &catt );

    TEST_ASSERT_EQUAL( CATT_SATISFIED , catt_state( &catt ) );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_catt_init_sets_starting_stats                  );
    RUN_TEST( test_catt_give_milk_increases_fullness              );
    RUN_TEST( test_catt_give_milk_caps_at_100                     );
    RUN_TEST( test_catt_stat_decrease_lowers_all_stats            );
    RUN_TEST( test_catt_stat_decrease_never_underflows_below_0    );
    RUN_TEST( test_catt_satisfied_is_false_if_any_stat_is_0       );
    RUN_TEST( test_catt_satisfied_is_true_if_all_stats_above_0    );
    RUN_TEST( test_catt_alive_is_false_only_if_all_stats_are_0    );
    RUN_TEST( test_catt_alive_is_true_if_even_one_stat_is_above_0 );
    RUN_TEST( test_catt_state_is_satisfied_if_all_stats_above_0   );
    RUN_TEST( test_catt_state_is_unsatisfied_if_some_but_not_all_stats_are_0 );
    RUN_TEST( test_catt_state_is_dead_if_all_stats_are_0          );
    RUN_TEST( test_catt_give_heart_increases_happiness            );
    RUN_TEST( test_catt_give_meds_increases_wellness              );
    RUN_TEST( test_catt_give_heart_and_meds_cap_at_100            );
    RUN_TEST( test_catt_stat_decrease_from_exactly_the_decay_amount_reaches_0 );
    RUN_TEST( test_catt_dies_after_ten_decays_with_no_care        );
    RUN_TEST( test_catt_state_is_unsatisfied_when_only_one_stat_is_left_above_0 );
    RUN_TEST( test_catt_state_is_unsatisfied_for_each_single_empty_stat );
    RUN_TEST( test_catt_state_is_satisfied_again_once_the_empty_stat_is_replenished );
    RUN_TEST( test_catt_valid_is_true_for_starting_stats          );
    RUN_TEST( test_catt_valid_is_true_at_the_max_of_100           );
    RUN_TEST( test_catt_valid_is_false_if_any_stat_is_above_100   );

    return UNITY_END();
}
