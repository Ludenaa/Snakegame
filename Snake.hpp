#pragma once
#define SNAKE_HPP

#include <deque>
#include <ncurses.h>

class Snake {
private:
    std::deque<Position> body; 
    int Dirction[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};

public:
    // 생성자: 시작 위치(y, x)와 초기 길이 설정
    Snake(int start_y, int start_x, int initial_length = 3);
    ~Snake();

    // Getter 함수들
    const std::vector<Position>& get_body() const;
    Position get_head() const;
    Direction get_direction() const;

    // 주요 기능 함수
    void change_direction(int dir); // 방향 전환
    void move(bool grow = false);             // 1칸 전진
};