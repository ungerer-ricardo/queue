add_executable( queue_test queue_test.cpp )


add_test(NAME queue_test COMMAND queue_test)

find_library(gtest_location NAMES libgtest.a)
add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${gtest_location})

find_library(gtestmain_location NAMES libgtest_main.a)
add_library(gtest_main STATIC IMPORTED)
set_target_properties(gtest_main PROPERTIES IMPORTED_LOCATION ${gtestmain_location})

target_link_libraries(queue_test PUBLIC gtest gtest_main)
