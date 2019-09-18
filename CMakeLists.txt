cmake_minimum_required(VERSION 3.7)
project(osoyanlib C)
set(CMAKE_C_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC)
    option(ENABLE_COVERAGE "Enable coverage" FALSE)

    if (ENABLE_COVERAGE)
        add_compile_options(--coverage -O0)
    endif()
endif()

FILE(GLOB_RECURSE OsoyanSource src/*.c)
add_library(osoyanlib STATIC ${OsoyanSource})
target_link_libraries(osoyanlib --coverage)

# add_compile_options("--coverage")
# link_libraries("--coverage")

enable_testing()
add_executable(tester test/test.c)
target_link_libraries(tester --coverage)
target_link_libraries(tester osoyanlib)
add_test(Tester tester)