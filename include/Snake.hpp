#pragma once

#include <deque>
#include <utility>
#include <ncurses.h>
#include "Gate.hpp"
#include "ScoreBoard.hpp"

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
    // 실드 아이템 보유 여부
    bool shield;
    // 게이트 통과 중 여부
    int passingGate;
    // ScoreBoard 포인터
    ScoreBoard* scoreBoard;

public:
    Snake(int x, int y, int snakesize, int firstdir, int (*p)[MAP_SIZE], Gate* g, ScoreBoard* sb);
    ~Snake();

    void changeDirection(int dir);
    bool move();
    void grow(int x, int y);
    void decrease();

    std::pair<int,int> getHead() const { return {body.back().first, body.back().second}; }

    // 뱀이 게이트를 통과하는 중인지 여부
    int isPassinggate() const;
};