cmake_minimum_required(VERSION 3.6)
project(proj3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -Wextra -lm")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES proj3.c)
add_executable(proj3 ${SOURCE_FILES})