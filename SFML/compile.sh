#!/bin/bash
g++ -o $* $*.cpp `sdl2-config --cflags --libs`
echo "Compiled!"
