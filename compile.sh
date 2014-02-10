#!/bin/bash
set -e

g++ -std=c++11 selector.cpp -lopencv_core -lopencv_highgui -Wl,-rpath -Wl,.
./a.out
