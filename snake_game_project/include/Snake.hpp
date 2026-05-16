#pragma once
#define SNAKE_HPP
#include <deque>
#include <utility>
#include <ncurses.h>

#define MAP_SIZE 100
/*
    todo : 진출 게이트의 좌표 받아오기
*/
class Snake {
private:
    std::deque<std::pair<int,int>> body; 
    // 0 1 2 3 순서대로 상, 우, 하, 좌
    int Dirction[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
    // 배열 저장 포인터
    int (*arr)[MAP_SIZE];
    //현재 방향
    int dir;
    // 이동할 위치 
    int dx,dy;
    //진입게이트와 진출게이트 좌표;
    std::pair<int,int> ingate, outgate;

public:
    Snake(int x, int y, int snakesize, int firstdir, int (*p)[MAP_SIZE]);
    ~Snake();

    void change_direction(int dir); // 방향 전환
    bool move();
    void grow(int x, int y);
    void decrease();

    void get_position();
};