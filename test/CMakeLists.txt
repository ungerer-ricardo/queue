include ( GoogleTest )

find_package(GTest REQUIRED)
include_directories( ${GTEST_INCLUDE_DIRS} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable( queue_test blocking_queue_test.cpp circular_buffer_queue_test.cpp ../cpp11logger/logger.cpp)
target_link_libraries(queue_test PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)

gtest_discover_tests( queue_test )

