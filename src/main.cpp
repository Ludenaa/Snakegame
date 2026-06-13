#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "ScoreBoard.hpp"
#include "Snake.hpp"
#include "Map.hpp"
#include "Gate.hpp"
#include "Item.hpp" 
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

    int game_difficulty = 1; //처음에 난이도를 입력받는 형태로 하는게 좋아보임

    /* 객체 생성 순서 주의: Gate → Snake 순서로 생성 */
    Map map(game_difficulty);
    ScoreBoard sb(8, 30, 0, (/*map.getWidth()*/ 21)*2 + 4); //맵 출력할때 (문자+1)씩 출력함 그래서 *2
    Gate gate(map.map, (/*map.getHeight()*/ 21), (/*map.getWidth()*/ 21));
    Snake snk(10, 10, 3, 1, map.map, &gate, &sb);

    // 난이도 반영
    sb.setDifficulty(game_difficulty);

    /* Item */
    Item item;

    // 초기 렌더링
    map.render();
    sb.render();

    //게임 시간 기록(점수 측정및 게이트 생성 타이밍용)
    std::chrono::steady_clock::time_point game_start = sb.getStartTime();
    std::chrono::steady_clock::time_point last_gate_spawn = game_start;
    //아이템 생성 타이밍용
    std::chrono::steady_clock::time_point last_item_spawn = game_start;

    while(true) {
        int ch = collectInput(200); // 200ms tick
        if(ch == 'q') break;
        int dir = getDirection(ch);

        // 방향 전환 (-1이면 방향키 외 입력 → 무시)
        if(dir != -1) snk.changeDirection(dir);

        if((1 /* 스네이크쪽에서 게이트를 지나고 있는지 active 리턴 받아서 사용 예정*/) 
        && (isExpired(last_gate_spawn, 10) || 0 /* 뱀의 길이가 10 이상인 경우(sb.getScore() >= 10) */)) {
            // 10초마다 게이트 생성
            gate.spawn();
            last_gate_spawn = std::chrono::steady_clock::now();
        }

        // 5초마다 아이템 생성
        if(isExpired(last_item_spawn,5)){
            item.CreateItem(map);
            last_item_spawn = std::chrono::steady_clock::now();
        }
        // 시간초과된 아이템 삭제
        item.removeExpiredItems(map);

        // 이동 실패 시 게임 오버
        if(!snk.move()) {
            sb.setEndTime();
            break;
        }

        //먹은 아이템을 리스트에서 제거
        std::pair<int, int> head = snk.getHead();
        item.clearItem(head.first, head.second);


        sb.updateSurvivalTime(); //생존 시간 업데이트

        map.render();
        sb.render();
    }

    endwin();
    std::cout << "game over" << std::endl;
    return 0;
}
