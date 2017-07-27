cmake_minimum_required(VERSION 3.8)
project(SnakeV2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES snake.c Snake2)
add_executable(SnakeV2 ${SOURCE_FILES})