#!/bin/bash
rm -r -f ./build && mkdir build
export OpenCV_DIR=/usr/include/opencv4
cd build && cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
   -DCMAKE_BUILD_TYPE=Debug ..
