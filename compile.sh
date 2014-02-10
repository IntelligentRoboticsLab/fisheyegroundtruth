#!/bin/bash
set -e

g++ -std=c++11 selector.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -Wl,-rpath -Wl,/usr/local/lib
./a.out test_data.png
