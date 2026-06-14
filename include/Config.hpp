/**
 * @file Config.hpp
 * @brief 게임 난이도 설정 정의
 */
#pragma once

enum class Difficulty { Easy = 1, Normal = 2, Hard = 3, Exetreme = 4};

struct MissionGoal { int length, growth, poison, gate; };  // ScoreBoard용
struct MapSize { int width, height; };  // Map용

struct GameConfig {
    Difficulty  level;
    int         map_num;        // Map  : 로드할 맵 번호
    int         tick_ms;        // 공통 : 게임 속도(ms/tick)
    MissionGoal mission;        // ScoreBoard : 미션 목표
    MapSize     map_size;       // Map : 맵 크기
};

GameConfig makeConfig(Difficulty d);   // 난이도 → 파라미터 해석 (단일 지점)
// GameConfig cgf = makeConfig(Difficulty::Easy)을 만들어
// cgf.tick_ms, cgf.mission.length 등으로 난이도별 설정값을 사용할 수 있습니다
