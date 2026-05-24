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

    

    //게임 난이도
    int const difficulty = 1; //ms 단위, 낮을수록 어려움s
    timeout(1000/(difficulty+1)); // 게임 루프마다 1000ms마다 키 입력 체크, 이걸로 게임 속도 조절



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
        int ch = getch();
        if(ch=='q') break;
        if(ch != ERR) {
            // 키를 입력하지 않아서 시간이 지나 입력을 못 받은 경우 ch는 ERR임. 이 경우 현재 방향 그대로 진행
            //스네이크 방향 설정및 이동
            snk.change_direction(get_direction(ch));
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