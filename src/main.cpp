#include <ncurses.h>
#include <iostream>
#include <chrono>
#include "ScoreBoard.hpp"
#include "Snake.hpp"
#include "Map.hpp"

using namespace std::chrono;

// 사용자 함수 (테스트 등)

// 0 1 2 3 순서대로 상, 우, 하, 좌
int get_direction(int ch){
    switch(ch) {
        case KEY_UP:
            return 0;
        case KEY_RIGHT:
            return 1;
        case KEY_DOWN:
            return 2;
        case KEY_LEFT:
            return 3;
        case 'q':
            return -1; // 'q' 입력 시 종료 신호
    }
    return ERR;// 예상치 못한 입력, 현재 방향으로 진행
}


/**
 * @brief 주어진 시간(ms) 동안 입력을 수집하고 마지막 입력을 반환
 * @param duration_ms 대기할 시간 (밀리초)
 * @return 마지막으로 입력된 키값, 입력 없으면 ERR
 */
int collect_input(int duration_ms) {
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
        if(ch == 'q') break; // 'q' 입력 시 즉시 종료
    }

    return last_input;
}


int main() {
    //ncurses 초기화
    initscr();          // ncurses 모드 시작
    cbreak();           // 엔터키 없이도 입력 즉시 반응하게 함
    curs_set(0);        // 커서를 화면에서 숨김

    // 전체 화면(stdscr)을 한 번 지우고 새로고침
    // 이걸 해줘야 하위 윈도우들이 정상적으로 출력됨
    refresh();

    

    //게임 난이도
    int const difficulty = 1; //ms 단위, 낮을수록 어려움


    /* 맵 객체 */
    Map map(difficulty);
    
    /* 스코어보드 객체 */
    //윈도우 높이, 윈도우 너비, Y 좌표, X 좌표
    ScoreBoard sb(8, 30, 0, 50);
    
    /* 스네이크 객체 */
    Snake snk(10, 10, 3, 1, map.map);


    /* Item */



    map.render();
    sb.render();
    

    keypad(stdscr, TRUE); // 특수 키 입력 활성화
    while(true){
        int last_input = collect_input(1000/(difficulty+3));

        if(last_input == 'q') break; // 'q' 입력 시 게임 종료
        // 특정 시간동안 입력을 수집, 해당 입력이 유효하면 해당 방향으로 전환
        if(last_input != ERR) {
            snk.change_direction(get_direction(last_input));
        }

        if(!snk.move()) break;

        //렌더링
        map.render();
        refresh();
        sb.render();
    }

    std::cout << "game over" << std::endl;


    //키를 누를 때까지 화면 유지
    getch();

    //ncurses 모드 종료
    endwin();
    return 0;
}