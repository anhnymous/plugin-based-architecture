#!/bin/bash

mkdir -p build-conan
cd build-conan
conan install .. --build=missing -pr desktop
conan build ..
