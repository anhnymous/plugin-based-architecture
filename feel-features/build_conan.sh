#!/bin/bash

mkdir -p build-conan && cd build-conan
conan install .. --build=missing -pr desktop -e TARGET=LINUX_PC -s build_type=Release
conan build ..
