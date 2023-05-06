cmake_minimum_required(VERSION 3.25)
project(HW2)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(HW2
        src/hw2.cpp
        src/hw2_output.c
        src/hw2_output.h
        src/Inputs.cpp
        src/Inputs.h
        src/Matrix.cpp
        src/Matrix.h src/Semaphore.cpp src/Semaphore.h)
