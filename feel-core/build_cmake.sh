#!/bin/bash

mkdir -p build-cmake
cd build-cmake
cmake .. -DTARGET=LINUX_PC
make
