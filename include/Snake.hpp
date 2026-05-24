#pragma once

#include <deque>
#include <utility>
#include <ncurses.h>
#include "Gate.hpp"

#define MAP_SIZE 100

class Snake {
private:
    std::deque<std::pair<int,int>> body;
    // 0 1 2 3 순서대로 상, 우, 하, 좌
    int Dirction[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
    // 배열 저장 포인터
    int (*arr)[MAP_SIZE];
    // Gate 포인터
    Gate* gate;
    // 현재 방향
    int dir;
    // 이동할 위치
    int dx, dy;

public:
    Snake(int x, int y, int snakesize, int firstdir, int (*p)[MAP_SIZE], Gate* g);
    ~Snake();

    void changeDirection(int dir);
    bool move();
    void grow(int x, int y);
    void decrease();

    std::pair<int,int> getHead() const { return {body.back().first, body.back().second}; }
};