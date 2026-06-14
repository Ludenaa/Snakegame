/**
 * @file ScoreBoard.hpp
 * @brief 게임 점수판과 미션 상태를 관리하는 ScoreBoard 클래스 선언
 */

#pragma once
#include <ncurses.h>
#include <chrono>
#include "Config.hpp"


class ScoreBoard {
private:
    WINDOW* scoreboard_win;     // ScoreBoard 윈도우 포인터
    WINDOW* mission_win;        // Mission 윈도우 포인터


    //점수 관련
    int current_length; //현재 길이
    int max_length;     //최대 길이
    int growth_cnt;     //획득한 Growth Items
    int poison_cnt;     //획득한 Poison Items
    int gate_cnt;       //gate 사용 횟수

    //난이도 관련
    const GameConfig& config;

    //시간 관련
    int survival_time;  //스네이크의 생존 시간(seconds)
    std::chrono::steady_clock::time_point start_time;     //게임 시작 시간
    std::chrono::steady_clock::time_point end_time;       //게임 종료 시간

public:
    // 생성자 및 소멸자
    ScoreBoard(int h, int w, int y, int x, const GameConfig& config);
    ~ScoreBoard();


    // double-free 차단
    // 복사를 delete하면 이동 생성자도 자동으로 생성되지 않으니(암묵 삭제) 추가 처리 불필요
    ScoreBoard(const ScoreBoard&) = delete;
    ScoreBoard& operator=(const ScoreBoard&) = delete;

    
    //adders
    void addGrowth();
    void addPoison(bool shielded); // shielded=true면 길이 감소 없이 카운트만 증가
    void addGateCnt();


    void resetScore();
    void render() const;
    void updateSurvivalTime();

    //getters & setters
    std::chrono::steady_clock::time_point getStartTime() const;
    void setEndTime();
    bool completeMission() const;

    //최종 결과 집계용 getters
    int getSurvivalTime() const;
    int getGrowthCnt() const;
    int getPoisonCnt() const;
    int getMaxLength() const;
};