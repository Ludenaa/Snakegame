#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "ScoreBoard.hpp"
#include "Snake.hpp"
#include "Map.hpp"
#include "Gate.hpp"

/**
 * @brief 키 입력을 방향(0~3)으로 변환
 *        0:상 1:우 2:하 3:좌, 방향키 외 입력은 -1 반환
 */
int getDirection(int ch) {
    switch(ch) {
        case KEY_UP:    return 0;
        case KEY_RIGHT: return 1;
        case KEY_DOWN:  return 2;
        case KEY_LEFT:  return 3;
    }
    return -1;
}

/**
 * @brief duration_ms 동안 입력을 수집하고 마지막 입력 반환
 *        입력이 들어와도 시간이 다 찰 때까지 대기
 */
int collectInput(int duration_ms) {
    auto start = std::chrono::steady_clock::now();
    int last_input = ERR;

    while(true) {
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        int remaining = duration_ms - elapsed;

        if(remaining <= 0) break;

        timeout(remaining);
        int ch = getch();
        if(ch != ERR) last_input = ch;
    }
    return last_input;
}

bool isExpired(const std::chrono::steady_clock::time_point& Last_time, const int startd_time) {
    auto now = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - Last_time).count();
    return elapsed >= startd_time;
}


int main() {
    srand(time(0)); // Gate spawn, Item 위치 랜덤 시드

    // ncurses 초기화
    initscr();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    refresh();

    /* 객체 생성 순서 주의: Gate → Snake 순서로 생성 */
    Map map(1);
    ScoreBoard sb(8, 30, 0, 50);
    Gate gate(map.map, map.getHight(), map.getWidth());
    Snake snk(10, 10, 3, 1, map.map, &gate);
    /* Item */


    // 초기 렌더링
    map.render();
    sb.render();

    //게임 시간 기록(점수 측정및 게이트 생성 타이밍용)
    auto game_start = std::chrono::steady_clock::now();
    auto last_gate_spawn = game_start;


    while(true) {
        int ch = collectInput(200); // 200ms tick
        if(ch == 'q') break;
        // 방향 전환 (-1이면 방향키 외 입력 → 무시)
        int dir = getDirection(ch);
        if(dir != -1) snk.changeDirection(dir);

        if((1 /* 스네이크쪽에서 게이트를 지나고 있는지 active 리턴 받아서 사용 예정*/) 
        && (isExpired(last_gate_spawn, 10) || 0 /* 뱀의 길이가 10 이상인 경우(sb.getScore() >= 10) */)) {
            // 10초마다 게이트 생성
            gate.spawn();
            last_gate_spawn = std::chrono::steady_clock::now();
        }

        // 이동 실패 시 게임 오버
        if(!snk.move()) break;

        map.render();
        sb.render();
    }

    endwin();
    std::cout << "game over" << std::endl;
    return 0;
}