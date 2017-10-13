cmake_minimum_required(VERSION 3.8)
project(pgrep)

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

SET_SOURCE_FILES_PROPERTIES( pgrep.c PROPERTIES LANGUAGE CXX )

set(SOURCE_FILES
        pgrep.c)

add_executable(pgrep ${SOURCE_FILES})
target_link_libraries(pgrep Threads::Threads)
