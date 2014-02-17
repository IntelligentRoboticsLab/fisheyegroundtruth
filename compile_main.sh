#!/bin/bash
set -e

g++ -std=c++11 main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -Wl,-rpath -Wl,/usr/local/lib -o blur
./blur ./manually_selected_field.png 8 8 
