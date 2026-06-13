#include <chrono>
#include "ScoreBoard.hpp"
#include "Config.hpp"

/**
 * @brief 윈도우 높이:h, 윈도우 너비:w, Y 좌표:y, X 좌표:x
 */
ScoreBoard::ScoreBoard(int h, int w, int y, int x, const GameConfig& config) : config(config) {
    scoreboard_win = newwin(h, w, y, x);
    scoreboard_size = {h,w,y,x};
    mission_win = newwin(h, w, y+10, x);
    mission_size = {h, w, y+10, x};

    resetScore();
    render();
}


/**
 * @brief 소멸자: 할당받은 윈도우 자원 해제
 */
ScoreBoard::~ScoreBoard() {
    delwin(scoreboard_win);
    delwin(mission_win);
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
    start_time    = std::chrono::steady_clock::now();
    end_time      = start_time;
    survival_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    //추후 Mission
}


void ScoreBoard::updateSurvivalTime() {
    survival_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count();
}


std::chrono::steady_clock::time_point ScoreBoard::getStartTime() const {
    return start_time;
}


void ScoreBoard::setEndTime() {
    end_time = std::chrono::steady_clock::now();
}


/**
 * @brief render: ScoreBoard와 Mission 윈도우에 현재 점수와 미션 상태를 출력
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
    mvwprintw(scoreboard_win, 6, 2, "T : %d", survival_time);

    //mission render line
    box(mission_win, 0, 0);
    mvwprintw(mission_win, 0, 8, "[ Mission ]"); 
    mvwprintw(mission_win, 2, 2, "B : (%d)\t(%s)", config.mission.length, (config.mission.length<=current_length) ? "V" : " ");
    mvwprintw(mission_win, 3, 2, "+ : (%d)\t(%s)", config.mission.growth, (config.mission.growth<=growth_cnt) ? "V" : " ");
    mvwprintw(mission_win, 4, 2, "- : (%d)\t(%s)", config.mission.poison, (config.mission.poison<=poison_cnt) ? "V" : " ");
    mvwprintw(mission_win, 5, 2, "G : (%d)\t(%s)", config.mission.gate, (config.mission.gate<=gate_cnt) ? "V" : " ");


    wrefresh(mission_win);
    wrefresh(scoreboard_win);
}