cmake_minimum_required(VERSION 3.15)
project(Lab1_v3)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Lab1_v3 ../main.cpp cmake-build-debug/pgmwork.cpp cmake-build-debug/pgmwork.h cmake-build-debug/ppmwork.cpp cmake-build-debug/ppmwork.h)