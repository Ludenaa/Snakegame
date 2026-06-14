/**
 * @file ScoreBoard.cpp
 * @brief 게임 점수판과 미션 상태를 관리하는 ScoreBoard 클래스 구현
 */

#include <chrono>
#include "ScoreBoard.hpp"
#include "Config.hpp"

/**
 * @brief 윈도우 높이:h, 윈도우 너비:w, Y 좌표:y, X 좌표:x
 */
ScoreBoard::ScoreBoard(int h, int w, int y, int x, const GameConfig& config) : config(config) {
    scoreboard_win = newwin(h, w, y, x);    scoreboard_size = {h,w,y,x};
    mission_win = newwin(h, w, y+10, x);    mission_size = {h, w, y+10, x};
    
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
    current_length++; //길이 증가

    //MAX update
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
    current_length = 3; //현재 길이(초기값 3)
    max_length = current_length;     //최대 길이(초기 길이 3 반영)
    growth_cnt = 0;     //획득한 Growth Items
    poison_cnt = 0;     //획득한 Poison Items
    gate_cnt = 0;       //gate 사용 횟수

    //시간          
    start_time    = std::chrono::steady_clock::now();
    end_time      = start_time;
    survival_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
}

/**
 * @brief 생존 시간 업데이트
 */
void ScoreBoard::updateSurvivalTime() {
    survival_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count();
}

/**
 * @brief 게임 시작 시간 반환, 게임 종료 시간 설정
 */
std::chrono::steady_clock::time_point ScoreBoard::getStartTime() const {
    return start_time;
}

/**
 * @brief 게임 종료 시간 설정, 생존 시간 최종 확정
 */
void ScoreBoard::setEndTime() {
    end_time = std::chrono::steady_clock::now();
    survival_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
}

/**
 * @brief 최종 결과 집계용 getters
 */
int ScoreBoard::getSurvivalTime() const { return survival_time; }
int ScoreBoard::getGrowthCnt()    const { return growth_cnt; }
int ScoreBoard::getPoisonCnt()    const { return poison_cnt; }
int ScoreBoard::getMaxLength()    const { return max_length; }

/**
 * @brief 미션 달성 여부 반환
 */
bool ScoreBoard::completeMission() const {
    return (current_length >= config.mission.length) &&
           (growth_cnt >= config.mission.growth) &&
           (poison_cnt >= config.mission.poison) &&
           (gate_cnt >= config.mission.gate);
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