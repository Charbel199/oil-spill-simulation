cmake_minimum_required(VERSION 3.16.0)
project(Oil_Spill_Simulation)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)
include_directories(src/parameters)

add_executable(Oil_Spill_Simulation
        src/parameters/parameters.cpp
        src/parameters/parameters.h
        src/run.cpp src/parameters/simulation_parameters.h src/parameters/simulation_parameters.cpp)
