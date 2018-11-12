cmake_minimum_required(VERSION 3.12)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code main.cpp inc/core/Particle.cpp inc/core/Particle.h inc/core/System.cpp inc/core/System.h
        inc/lib/jsoncpp/json.h inc/lib/jsoncpp/jsoncpp.cpp inc/core/Reaction.cpp inc/core/Reaction.h)