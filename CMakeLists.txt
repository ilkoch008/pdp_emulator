cmake_minimum_required(VERSION 3.10)
project(test_3 C)

set(CMAKE_C_STANDARD 99)

add_executable(test_3 main.c variables.h rubbish.c functions.c take.c)