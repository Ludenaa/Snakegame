/**
 * @file Snake.hpp
 * @brief Snake의 몸통·이동·방향 전환 및 벽/아이템/게이트와의 충돌 처리 인터페이스 선언
 */
#pragma once

#include <deque>
#include <utility>
#include <ncurses.h>
#include "Gate.hpp"
#include "ScoreBoard.hpp"
#include "Map.hpp"   // MAP_SIZE 정의 사용

class Snake {
private:
    std::deque<std::pair<int,int>> body;
    // 0 1 2 3 순서대로 상, 우, 하, 좌
    int direction[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
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
    int passing_gate;
    // ScoreBoard 포인터
    ScoreBoard* score_board;

public:
    Snake(int x, int y, int snake_size, int first_dir, int (*p)[MAP_SIZE], Gate* g, ScoreBoard* sb);
    ~Snake();

    void changeDirection(int next_dir);
    bool move();
    void grow(int x, int y);
    void decrease();

    std::pair<int,int> getHead() const { return {body.back().first, body.back().second}; }

    // 뱀이 게이트를 통과하는 중인지 여부
    int isPassinggate() const;

    // 실드 아이템 보유 여부
    bool hasShield() const { return shield; }
};