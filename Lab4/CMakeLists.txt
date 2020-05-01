cmake_minimum_required(VERSION 3.15)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Lab4 main.cpp convertions.cpp convertions.h)