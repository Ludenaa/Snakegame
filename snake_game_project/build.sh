#!/bin/bash

# build 폴더가 없으면 생성
if [ ! -d "build" ]; then
  mkdir build
fi

# bin 폴더가 없으면 생성
if [ ! -d "bin" ]; then
  mkdir bin
fi

# 3. 빌드 진행
cd build
cmake ..
make

# 4. 성공 시 실행
if [ $? -eq 0 ]; then
    echo "----------------------------"
    echo "Build Success, Running Game..."
    echo "----------------------------"
    ../bin/SnakeGame
else
    echo "Build Failed."
fi