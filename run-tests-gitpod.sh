#!/bin/bash

cd cpp

cmake_flags="-DCMAKE_BUILD_TYPE=Release -DLINKS_PLATFORM_TESTS=ON"
cmake_build_dir="build"
mkdir -p $cmake_build_dir && cd $cmake_build_dir
conan install .. -pr=linksplatform --build=missing
cmake .. "$cmake_flags"
cmake --build .
scan-build cmake --build .
binaries=bin/*
for binary in $binaries
do
   ./$binary
done
