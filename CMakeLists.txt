cmake_minimum_required(VERSION 3.12)
project(MD)

set(CMAKE_CXX_STANDARD 17)

add_executable(MD main.cpp inc/core/System.cpp inc/core/System.h inc/core/Particle.cpp inc/core/Particle.h)