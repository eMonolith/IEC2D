#!/bin/sh
echo compiling...
g++ iec2d-0.01-04.c -Wall -o iec2d-0.01-04 `sdl-config --cflags --libs` -lSDL -lSDL_image -lSDL_ttf -lm -no-pie
echo done.
