cmake_minimum_required(VERSION 3.1)
project(bosh_queue)

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set (CMAKE_CXX_STANDARD 20)

include_directories(queue cpp11logger)

add_executable(bosh_queue prog/main.cpp cpp11logger/logger.cpp)

TARGET_LINK_LIBRARIES( bosh_queue LINK_PUBLIC ${Boost_LIBRARIES} Threads::Threads)

enable_testing()
add_subdirectory(test)
