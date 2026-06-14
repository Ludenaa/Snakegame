#!/bin/bash


if [ ! -d "build" ]; then
  mkdir build
fi

if [ ! -d "bin" ]; then
  mkdir bin
fi

cd build || exit 1

if ! cmake ..; then
    echo "----------------------------"
    echo "CMake configuration failed."
    echo "----------------------------"
    exit 1
fi

if make; then
    echo "----------------------------"
    echo "Build Success, Running Game..."
    echo "----------------------------"
    ../bin/SnakeGame
else
    echo "----------------------------"
    echo "Build Failed."
    echo "----------------------------"
    exit 1
fi