cmake_minimum_required(VERSION 3.25)
project(PathsMgr)

set(CMAKE_CXX_STANDARD 17)

set(HEADER_FILES
        DiskDataIO.h
        PathsMgr.h
        ArgumentsParser.h
        common.h
        )

set(SOURCE_FILES
        DiskDataIO.cpp
        PathsMgr.cpp
        ArgumentsParser.cpp
        common.cpp)

# build目录下在第一次执行cmake .. 时，由于CMakeCache.txt还未生成，gtest_SOURCE_DIR的值是空
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${googletest}/include ${googletest})


add_library(PathsMgr_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})