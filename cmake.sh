#!/bin/bash
rm -r -f ./build && mkdir build
cd build && cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
   -DCMAKE_BUILD_TYPE=Debug ..
