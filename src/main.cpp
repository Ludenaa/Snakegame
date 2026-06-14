/**
 * @file main.cpp
 * @brief 게임 진입점 - ncurses 초기화, 난이도별 스테이지 진행 및 메인 게임 루프 구동
 */
#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "ScoreBoard.hpp"
#include "Snake.hpp"
#include "Map.hpp"
#include "Gate.hpp"
#include "Item.hpp"
#include "Config.hpp"

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

/**
 * @brief 한 스테이지의 실행 결과
 *        Cleared: 미션 달성, GameOver: 사망, Quit: 사용자 종료(q)
 */
enum class StageResult { Cleared, GameOver, Quit };

/**
 * @brief 난이도 이름 문자열 반환 (전환 화면 출력용)
 */
const char* difficultyName(Difficulty d) {
    switch(d) {
        case Difficulty::Easy:   return "Easy";
        case Difficulty::Normal: return "Normal";
        case Difficulty::Hard:   return "Hard";
        case Difficulty::Extreme: return "Extreme";
    }
    return "Unknown";
}

/**
 * @brief 미션 클리어 후 다음 스테이지 안내를 화면 중앙에 약 3초간 출력
 * @param next 다음에 진행할 난이도
 */
void showStageTransition(Difficulty next) {
    clear();

    int rows, cols;
    getmaxyx(stdscr, rows, cols); // 터미널 크기 → 가운데 좌표 계산
    int cy = rows / 2;

    const char* l1 = "MISSION COMPLETE";
    const char* l2 = "Next Stage is";
    const char* l3 = difficultyName(next);

    // 각 줄을 가로 중앙에 배치
    mvprintw(cy - 1, (cols - (int)std::strlen(l1)) / 2, "%s", l1);
    mvprintw(cy,     (cols - (int)std::strlen(l2)) / 2, "%s", l2);
    mvprintw(cy + 1, (cols - (int)std::strlen(l3)) / 2, "%s", l3);
    refresh();

    napms(3000);  // 3초 대기
    flushinp();   // 대기 중 눌린 키 입력 버퍼 비우기(다음 스테이지 오작동 방지)
}

/**
 * @brief 주어진 난이도 설정으로 한 스테이지를 실행
 *        스테이지마다 객체를 새로 생성하므로 별도의 초기화 로직이 불필요
 * @param config 난이도별 설정값(맵, 미션, 틱 속도 등)
 * @return 스테이지 실행 결과(Cleared/GameOver/Quit)
 */
StageResult playStage(const GameConfig& config) {
    /* 객체 생성 순서 주의: Gate → Snake 순서로 생성 */
    Map map(config.map_num); //config 래퍼로 받아서 내부에서 사용 바랍니다!
    ScoreBoard sb(8, 30, 0, config.map_size.width * 2 + 4, config); //맵 출력할때 (문자+1)씩 출력함 그래서 *2
    Gate gate(map.map, config.map_size.height, config.map_size.width);
    Snake snk(10, 10, 3, 1, map.map, &gate, &sb);
    Item item;

    // 이전 스테이지 화면 잔상 제거 후 초기 렌더링
    clear();
    refresh();
    map.render();
    sb.render();

    //게임 시간 기록(점수 측정및 게이트 생성 타이밍용)
    std::chrono::steady_clock::time_point game_start = sb.getStartTime();
    std::chrono::steady_clock::time_point last_gate_spawn = game_start;

    //아이템 생성 타이밍용
    std::chrono::steady_clock::time_point last_item_spawn = game_start;

    while(true) {
        int ch = collectInput(config.tick_ms);
        if(ch == 'q') return StageResult::Quit;
        int dir = getDirection(ch);

        // 방향 전환 (-1이면 방향키 외 입력 → 무시)
        if(dir != -1) snk.changeDirection(dir);

        if((!snk.isPassinggate()) //스네이크 통과중에는 게이트 생성X(SPAWN에서 GATE 삭제도 호출하므로 SPAWN만 제어하면됨)
        && (isExpired(last_gate_spawn, 10))) {
            // 10초마다 게이트 생성
            gate.spawn();
            last_gate_spawn = std::chrono::steady_clock::now();
        }

        // 5초마다 아이템 생성
        if(isExpired(last_item_spawn,5)){
            item.CreateItem(map,config.level);
            last_item_spawn = std::chrono::steady_clock::now();
        }
        // 시간초과된 아이템 삭제
        item.removeExpiredItems(map);

        // 이동 실패 시 게임 오버
        if(!snk.move()) {
            sb.setEndTime();
            return StageResult::GameOver;
        }

        //먹은 아이템을 리스트에서 제거
        std::pair<int, int> head = snk.getHead();
        item.clearItem(head.first, head.second);


        sb.updateSurvivalTime(); //생존 시간 업데이트

        map.render();
        sb.render();

        // 미션 달성 시 현재 스테이지 클리어
        if(sb.completeMission()) {
            sb.setEndTime();
            return StageResult::Cleared;
        }
    }
}


int main() {
    srand(time(0)); // Gate spawn, Item 위치 랜덤 시드

    // ncurses 초기화 (게임 전체에서 1회만 수행)
    initscr();
    cbreak();
    noecho();   // 방향키 외 입력이 화면에 찍혀 맵이 깨지는 것 방지
    curs_set(0);
    keypad(stdscr, TRUE);
    refresh();

    // 난이도 진행 순서: 쉬움 → 보통 → 어려움 → 최종 
    const Difficulty stages[] = { Difficulty::Easy, Difficulty::Normal, Difficulty::Hard, Difficulty::Extreme };
    const int stage_count = sizeof(stages) / sizeof(stages[0]);

    bool cleared_all = true; // 모든 스테이지를 클리어했는지 여부

    for(int i = 0; i < stage_count; i++) {
        // 현재 난이도 설정 생성
        // (config는 ScoreBoard가 참조로 보관하므로 playStage보다 오래 살아있어야 함 → 여기서 생성)
        GameConfig config = makeConfig(stages[i]);

        StageResult result = playStage(config);

        // 클리어가 아니면(사망 또는 q 종료) 게임 종료
        if(result != StageResult::Cleared) {
            cleared_all = false;
            break;
        }

        // 현재 스테이지 클리어
        // 다음 스테이지가 있으면 전환 화면 출력 후 계속, 없으면(어려움 클리어) 루프 종료
        if(i + 1 < stage_count) {
            showStageTransition(stages[i + 1]);
        }
    }

    endwin(); // ncurses 종료

    // 최종 결과 출력
    if(cleared_all) {// 어려움까지 모두 클리어
        std::cout << "you win" << std::endl; 
    } else {
        std::cout << "game over" << std::endl;
    }
    return 0;
}
