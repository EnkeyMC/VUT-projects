cmake_minimum_required(VERSION 3.6)
project(vut_fit_project_1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror -Wextra -pedantic")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic")

set(SOURCE_FILES main.c)
add_executable(vut_fit_project_1 ${SOURCE_FILES})