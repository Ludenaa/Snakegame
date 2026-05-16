// ScoreBoard.hpp
#pragma once
#define SCOREBOARD_HPP

#include <ncurses.h>

struct WinSize{
    //윈도우 높이:h, 윈도우 너비:w, Y 좌표:y, X 좌표:x
    int h, w, y, x;
};

/*
    todo : Scoreboard가 관리하는 Mission 구현
*/

class ScoreBoard {
public:
    WINDOW* ScoreBoard_win;     // ncurses 전용 윈도우 포인터
    WinSize ScoreBoard_size;   //ScoreBoard의 사이즈 정보
    WinSize Mission_size;      //Mission의 사이즈 정보(상호 호출 가능 수준까지 오면 적용 예정)

    //점수 관련
    int current_length; //현재 길이
    int max_length;     //최대 길이
    int growth_cnt;     //획득한 Growth Items
    int poison_cnt;     //획득한 Poison Items
    int gate_cnt;       //gate 사용 횟수

    int survival_time;  //스네이크의 생존 시간
    int start_time;     //게임 시작 시간
    int end_time;       //게임 종료 시간
    

    // 생성자 및 소멸자
    ScoreBoard(int h, int w, int y, int x);
    ~ScoreBoard();

    
    //주요 함수
    void addScore(int point);
    void addGateCnt();
    void resetScore();
    void render();
};