cmake_minimum_required(VERSION 3.0)

PROJECT(MathPic)
aux_source_directory(server SERVER_SRC)
aux_source_directory(client CLIENT_SRC)

add_executable(Server ${SERVER_SRC})
add_executable(Client ${CLIENT_SRC})

TARGET_LINK_LIBRARIES(Server pthread)
TARGET_LINK_LIBRARIES(Client pthread)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")
