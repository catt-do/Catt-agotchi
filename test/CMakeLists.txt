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
