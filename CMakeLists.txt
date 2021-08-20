cmake_minimum_required(VERSION 2.8)
project(bosh_queue)

FIND_PACKAGE( Boost 1.76.0 COMPONENTS program_options REQUIRED )

set (CMAKE_CXX_STANDARD 20)

include_directories(queue cpp11logger)

add_executable(bosh_queue prog/main.cpp cpp11logger/logger.cpp)

TARGET_LINK_LIBRARIES( bosh_queue LINK_PUBLIC ${Boost_LIBRARIES} )

enable_testing()
add_subdirectory(test)
