#include "ScoreBoard.hpp"

/**
 * @brief 윈도우 높이:h, 윈도우 너비:w, Y 좌표:y, X 좌표:x
 */
ScoreBoard::ScoreBoard(int h, int w, int y, int x) {
    resetScore();
    ScoreBoard_win = newwin(h, w, y, x);
    ScoreBoard_size = {h,w,y,x};
    render();
}

/**
 * @brief 소멸자: 할당받은 윈도우 자원 해제
 */
ScoreBoard::~ScoreBoard() {
    delwin(ScoreBoard_win);
}


/**
 * @brief 점수 추가 및 최고 점수 갱신(B, +, -)
 */
void ScoreBoard::addScore(int point) {
    //Item(growth, poison)을 먹은 경우 호출
    if(point == 1)  growth_cnt++;
    else            poison_cnt++;

    //MAX update
    current_length += point;
    if (current_length > max_length) {
        max_length = current_length;
    }

    //일단 렌더를 따로 함, 추후 협의 필요
    render();
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

    //추후 Mission
}


/**
 * @brief 화면에 출력
 */
void ScoreBoard::render() {
    werase(ScoreBoard_win);

    //ScoreBoard_win render line
    box(ScoreBoard_win, 0, 0);
    mvwprintw(ScoreBoard_win, 0, 8, "[ ScoreBoard ]");
    mvwprintw(ScoreBoard_win, 2, 2, "B : (%d) / (%d)", current_length, max_length);
    mvwprintw(ScoreBoard_win, 3, 2, "+ : %d", growth_cnt);
    mvwprintw(ScoreBoard_win, 4, 2, "- : %d", poison_cnt);
    mvwprintw(ScoreBoard_win, 5, 2, "G : %d", gate_cnt);
    wrefresh(ScoreBoard_win);
}