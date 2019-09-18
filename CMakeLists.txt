cmake_minimum_required(VERSION 3.7)
project(osoyanlib C)

set(CMAKE_C_STANDARD 11)

FILE(GLOB_RECURSE OsoyanSource src/*.c)
add_library(osoyanlib STATIC ${OsoyanSource})

add_compile_options("--coverage")
link_libraries("--coverage")

enable_testing()
add_executable(tester test/test.c)
add_test(Tester tester)