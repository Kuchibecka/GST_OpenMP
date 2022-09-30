cmake_minimum_required(VERSION 3.23)
project(GST_OpenMP)

set(CMAKE_CXX_STANDARD 20)

add_executable(GST_OpenMP main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")