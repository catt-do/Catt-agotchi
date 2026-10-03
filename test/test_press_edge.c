#include "unity.h"
#include "press_edge.h"

void setUp( void )
{
    /* Nothing to setup */
}

void tearDown( void )
{
    /* Nothing to teardown */
}

void test_press_edge_fires_when_button_goes_down( void )
{
    bool was_down = false;

    TEST_ASSERT_TRUE( button_press_edge( true , &was_down ) );
}

void test_press_edge_fires_only_once_while_held( void )
{
    bool was_down = false;

    TEST_ASSERT_TRUE(  button_press_edge( true , &was_down ) );
    TEST_ASSERT_FALSE( button_press_edge( true , &was_down ) );
    TEST_ASSERT_FALSE( button_press_edge( true , &was_down ) );
}

void test_press_edge_does_not_fire_while_released( void )
{
    bool was_down = false;

    TEST_ASSERT_FALSE( button_press_edge( false , &was_down ) );
    TEST_ASSERT_FALSE( button_press_edge( false , &was_down ) );
}

void test_press_edge_does_not_fire_on_release( void )
{
    bool was_down = true;

    TEST_ASSERT_FALSE( button_press_edge( false , &was_down ) );
}

void test_press_edge_fires_again_after_release_and_repress( void )
{
    bool was_down = false;

    TEST_ASSERT_TRUE(  button_press_edge( true  , &was_down ) );
    TEST_ASSERT_FALSE( button_press_edge( false , &was_down ) );
    TEST_ASSERT_TRUE(  button_press_edge( true  , &was_down ) );
}

void test_press_edge_updates_was_down( void )
{
    bool was_down = false;

    button_press_edge( true , &was_down );
    TEST_ASSERT_TRUE( was_down );

    button_press_edge( false , &was_down );
    TEST_ASSERT_FALSE( was_down );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_press_edge_fires_when_button_goes_down           );
    RUN_TEST( test_press_edge_fires_only_once_while_held            );
    RUN_TEST( test_press_edge_does_not_fire_while_released          );
    RUN_TEST( test_press_edge_does_not_fire_on_release              );
    RUN_TEST( test_press_edge_fires_again_after_release_and_repress );
    RUN_TEST( test_press_edge_updates_was_down                      );

    return UNITY_END();
}
