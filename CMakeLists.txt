cmake_minimum_required(VERSION 3.6)
project(IZP_project2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -Wextra -lm")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lm")

set(SOURCE_FILES main.c)
add_executable(IZP_project2 ${SOURCE_FILES})