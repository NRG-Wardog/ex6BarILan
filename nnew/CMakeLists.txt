cmake_minimum_required(VERSION 3.29)
project(ex6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex6
    ex6.c
    ex6.h
    input.txt)
