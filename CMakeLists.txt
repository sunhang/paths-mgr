cmake_minimum_required(VERSION 3.16)
project(path_mgr)

set(CMAKE_CXX_STANDARD 17)

add_executable(PathsMgr_run main.cpp)

include_directories(PathsMgr_lib)

#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# build目录下在第一次执行cmake .. 时，由于CMakeCache.txt还未生成，gtest_SOURCE_DIR的值是空
if (DEFINED gtest_SOURCE_DIR)
    set(googletest ${gtest_SOURCE_DIR})
else ()
    set(googletest Google_tests/lib/googletest)
endif ()
include_directories(${googletest}/include ${googletest})

# 要放在add_subdirectory(Google_tests)前边
option(INSTALL_GMOCK "Install Googletest GMock?" OFF)
option(INSTALL_GTEST "Install Googletest GTest?" OFF)

add_subdirectory(PathsMgr_lib)
add_subdirectory(Google_tests)

target_link_libraries(PathsMgr_run PathsMgr_lib)

install(
        TARGETS PathsMgr_run
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(PROGRAMS prj_paths_mgr.sh
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        RENAME paths_mgr.sh)