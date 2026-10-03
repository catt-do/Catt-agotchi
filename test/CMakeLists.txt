cmake_minimum_required( VERSION 3.20 )
project( host_tests C )

enable_testing()

set( IDF_PATH "$ENV{IDF_PATH}" )

add_library(
    unity STATIC
    ${IDF_PATH}/components/unity/unity/src/unity.c
)

add_executable(
    test_catt
    test_catt.c
    ../components/catt/catt.c
)

target_include_directories(
    test_catt PRIVATE
    ../components/catt
    ${IDF_PATH}/components/unity/unity/src
)

target_link_libraries( test_catt PRIVATE unity )


add_test(
    NAME    test_catt
    COMMAND test_catt
)

add_executable(
    test_press_edge
    test_press_edge.c
    ../components/buttons/press_edge.c
)

target_include_directories(
    test_press_edge PRIVATE
    ../components/buttons
    ${IDF_PATH}/components/unity/unity/src
)

target_link_libraries( test_press_edge PRIVATE unity )

add_test(
    NAME    test_press_edge
    COMMAND test_press_edge
)

add_executable(
    test_decay_timer
    test_decay_timer.c
    ../components/catt/decay_timer.c
)

target_include_directories(
    test_decay_timer PRIVATE
    ../components/catt
    ${IDF_PATH}/components/unity/unity/src
)

target_link_libraries( test_decay_timer PRIVATE unity )

add_test(
    NAME    test_decay_timer
    COMMAND test_decay_timer
)

add_executable(
    test_sprite
    test_sprite.c
    ../components/display/sprite.c
)

target_include_directories(
    test_sprite PRIVATE
    ../components/display
    ${IDF_PATH}/components/unity/unity/src
)

target_link_libraries( test_sprite PRIVATE unity )

add_test(
    NAME    test_sprite
    COMMAND test_sprite
)
