#!/bin/bash
echo "$(date) | start build"

cmake -S ./src -B ./build
cmake --build build

echo "$(date) | finish build"
cd build
echo "$(date) | start runing with $@"

./ray-tracing $@

echo "$(date) | finish runing"
cd ..
