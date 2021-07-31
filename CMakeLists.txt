cmake_minimum_required(VERSION 2.8)
project(bosh_queue)

set (CMAKE_CXX_STANDARD 17)

include_directories(queue)

add_subdirectory(test)

add_executable(bosh_queue prog/main.cpp)

