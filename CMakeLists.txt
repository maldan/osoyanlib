cmake_minimum_required(VERSION 3.7)
project(osoyanlib C)
set(CMAKE_C_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC)
    option(ENABLE_COVERAGE "Enable coverage" FALSE)

    if (ENABLE_COVERAGE)
        add_compile_options(--coverage -O0)
    endif()
endif()

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -DDEBUG)
add_compile_options(-Wall -Wextra -Wpedantic)

FILE(GLOB_RECURSE OsoyanSource src/*.c)
add_library(osoyanlib STATIC osoyan.c ${OsoyanSource})
target_link_libraries(osoyanlib --coverage)

enable_testing()
add_executable(tester test/test.c)
target_link_libraries(tester --coverage)
target_link_libraries(tester osoyanlib)

add_test(StringTest1 tester string_basic)
add_test(StringTest2 tester string_format)
add_test(StringTest3 tester string_array)
add_test(OtherTest tester other)