#!/bin/sh
echo compiling...
g++ iec2d-0.01-08.c -Wall -o iec2d-0.01-08 `sdl-config --cflags --libs` -lSDL -lSDL_image -lSDL_ttf -lm -no-pie -O2
echo done.
