cmake_minimum_required(VERSION 3.7)
project(ROP)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -std=c++14 -std=gnu++14 -Wdeprecated -pthread -O3")

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp genetics.cpp Tabou.cpp Tabou.h)
add_executable(ROP ${SOURCE_FILES})