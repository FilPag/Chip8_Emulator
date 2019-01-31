#!/bin/bash
g++ -o $* $*.cpp -lsfml-graphics -lsfml-window -lsfml-system
echo "Compiled!"
