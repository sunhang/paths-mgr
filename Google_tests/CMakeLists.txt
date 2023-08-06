project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run PathsMgrTests.cpp arguments_parser_test.cpp diskdata_io_tests.cpp common_tests.cpp base_tests.h)

target_link_libraries(Google_Tests_run PathsMgr_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)


