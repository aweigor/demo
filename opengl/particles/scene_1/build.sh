#!/bin/sh

# only in macos
g++ -std=c++11 main.cpp -framework OpenGL -framework GLUT -o particle_system
