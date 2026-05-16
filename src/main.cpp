#include <ncurses.h>
#include <iostream>
#include "ScoreBoard.hpp"
#include "Snake.hpp"
#include "Map.hpp"

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
            return 0;
    }
    return 0;// 예상치 못한 입력
}


int main() {
    //ncurses 초기화
    initscr();          // ncurses 모드 시작
    cbreak();           // 엔터키 없이도 입력 즉시 반응하게 함
    curs_set(0);        // 커서를 화면에서 숨김

    // 전체 화면(stdscr)을 한 번 지우고 새로고침
    // 이걸 해줘야 하위 윈도우들이 정상적으로 출력됨
    refresh();

    //맵 렌더링
    Map map(1);
    map.render();

    /* test */
    //윈도우 높이, 윈도우 너비, Y 좌표, X 좌표
    ScoreBoard sb(8, 30, 0, 50);
    sb.render();

    //스네이크 객체 생성
    Snake snk(10, 10, 3, 1, map.map);


    keypad(stdscr, TRUE); // 특수 키 입력 활성화
    while(1){
        int ch = getch();     // 키 입력 받기
        if(ch=='q') break;

        //스네이크 방향 설정및 이동
        snk.change_direction(get_direction(ch));
        if(!snk.move()) break;

        //맵 렌더링
        map.render();
        refresh();
        sb.render();
    }

    std::cout << "game over" << std::endl;


    //키를 누를 때까지 화면 유지
    //getch();

    //ncurses 모드 종료
    endwin();
    return 0;
}