#!/bin/sh
# Unity build script for raylib projects
g++ main.cpp -g -c -I /opt/raylib/src -o main.o
g++ main.o -s -Wall -I/opt/raylib/src -L/opt/raylib/release/libs/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o gol
