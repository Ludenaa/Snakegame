#include "ScoreBoard.hpp"

/**
 * @brief 윈도우 높이:h, 윈도우 너비:w, Y 좌표:y, X 좌표:x
 */
ScoreBoard::ScoreBoard(int h, int w, int y, int x) {
    scoreboard_win = newwin(h, w, y, x);
    scoreboard_size = {h,w,y,x};
    mission_size = {0, 0, 0, 0};
    resetScore();
    render();
}

/**
 * @brief 소멸자: 할당받은 윈도우 자원 해제
 */
ScoreBoard::~ScoreBoard() {
    delwin(scoreboard_win);
}


/**
 * @brief Growth Item 획득 횟수 추가및 현재 길이 증가, 최대 길이 갱신
 */
void ScoreBoard::addGrowth() {
    growth_cnt++;
    
    //MAX update
    current_length++; //길이 증가
    if (current_length > max_length) {
        max_length = current_length;
    }
}

/**
 * @brief Poison Item 획득 횟수 추가
 */
void ScoreBoard::addPoison() {
    poison_cnt++; //획득한 Poison Items 횟수 추가
    current_length--; //현재 길이 감소
}

/**
 * @brief 게이트 탑승 횟수 추가
 */
void ScoreBoard::addGateCnt() {
    gate_cnt++;
}
 

/**
 * @brief 점수 초기화
 */
void ScoreBoard::resetScore() {
    current_length = 0; //현재 길이
    max_length = 0;     //최대 길이
    growth_cnt = 0;     //획득한 Growth Items
    poison_cnt = 0;     //획득한 Poison Items
    gate_cnt = 0;       //gate 사용 횟수

    //시간
    survival_time = 0;
    start_time    = 0;
    end_time      = 0;

    //추후 Mission
}


/**
 * @brief 화면에 출력
 */
void ScoreBoard::render() const{
    werase(scoreboard_win);

    //scoreboard_win render line
    box(scoreboard_win, 0, 0);
    mvwprintw(scoreboard_win, 0, 8, "[ ScoreBoard ]");
    mvwprintw(scoreboard_win, 2, 2, "B : (%d) / (%d)", current_length, max_length);
    mvwprintw(scoreboard_win, 3, 2, "+ : %d", growth_cnt);
    mvwprintw(scoreboard_win, 4, 2, "- : %d", poison_cnt);
    mvwprintw(scoreboard_win, 5, 2, "G : %d", gate_cnt);
    wrefresh(scoreboard_win);
}