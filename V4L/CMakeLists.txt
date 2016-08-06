cmake_minimum_required(VERSION 3.0)

PROJECT(V4L)
add_executable(V4L main.cpp video.cpp video.h)
TARGET_LINK_LIBRARIES(V4L pthread)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")
