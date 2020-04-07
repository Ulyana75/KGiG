cmake_minimum_required(VERSION 3.15)
project(Lab3)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Lab3 main.cpp)