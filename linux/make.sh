#!/bin/bash
g++-7 -o mycelle -std=c++17 \
 ./main.cpp \
 ./inc/core/Particle.h \
 ./inc/core/Particle.cpp \
 ./inc/core/System.h \
 ./inc/core/System.cpp \
 ./inc/core/Reaction.h \
 ./inc/core/Reaction.cpp \
 ./inc/lib/jsoncpp/json.h \
 ./inc/lib/jsoncpp/jsoncpp.cpp
