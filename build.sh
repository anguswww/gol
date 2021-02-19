g++ main.cpp -g -c -I /opt/raylib/src -o main.o
g++ main.o -s -Wall -std=c99 -I/opt/raylib/src -L/opt/raylib/release/libs/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o gol