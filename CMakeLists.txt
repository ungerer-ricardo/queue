cmake_minimum_required(VERSION 2.8)

project(bosh_queue)

include_directories(queue)

add_executable(bosh_queue prog/main.cpp)
