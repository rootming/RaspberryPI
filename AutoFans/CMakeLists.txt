cmake_minimum_required(VERSION 3.0)

PROJECT(Fans)
add_executable(Fans main.cpp)
TARGET_LINK_LIBRARIES(Fans wiringPi pthread)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")