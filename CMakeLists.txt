cmake_minimum_required(VERSION 3.9)
project(osoyanlib C)

set(CMAKE_C_STANDARD 11)

FILE(GLOB_RECURSE OsoyanSource src/*.c)
add_library(osoyanlib STATIC ${OsoyanSource})