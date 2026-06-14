/**
 * @file Map.hpp
 * @brief 스네이크 게임의 맵 데이터 구조 정의 및 오브젝트 배치/조회를 위한 인터페이스 제공
 */

#pragma once
#define MAP_HPP

#include <vector>
#include <string>
#include <utility> // std::pair 구조체 사용을 위해 추가
#include "Config.hpp" // ⭐ [변경] Config의 GameConfig 및 MapSize 설정을 참조하기 위해 헤더 추가

// 맵 배열의 한 변 크기(고정). Map의 map 배열과 Snake/Gate가 받는 배열 포인터에 공통 사용
constexpr int MAP_SIZE = 100;

enum TileType {
    EMPTY = 0,
    WALL = 1,
    IMMUNE_WALL = 2,
    SNAKE_HEAD = 3,
    SNAKE_BODY = 4,
    GROWTH = 5,
    POISON = 6,
    GATE = 7,
    SHIELD = 8 // ⭐ Item.cpp에서 새롭게 정의한 Shield 아이템 타입 추가
};

class Map {
private:
    // ⭐ [변경] 기존의 하드코딩된 내부 맵 크기 매핑 배열 제거
    // const int MAP_SIZES[5][2] = {{0, 0}, {21, 21}, {25, 25}, {31, 31}, {35, 35}}; 

    int width;
    int height;
    int map_num;

    // [성능 최적화]
    // 이전 프레임의 맵 상태를 기록하여 변경된 칸만 다시 그리기 위한 버퍼
    int prev_map[MAP_SIZE][MAP_SIZE];
    // 맵이 처음 로드되거나 변경되었을 때 전체 화면을 강제로 새로고침하기 위한 플래그
    bool force_redraw;

    bool loadMapFile();

public:
    // ⭐ [변경] 기존 Map(int map_num) 대신 Config의 중앙 집중형 설정을 받아오도록 생성자 변경
    Map(const GameConfig& config);
    
    int map[MAP_SIZE][MAP_SIZE];

    void mapClear();
    bool render();

    int getWidth() const;
    int getHeight() const;
    
    bool is_empty(int x, int y) const { return map[x][y] == EMPTY; }

    /**
     * @brief 맵 내부의 빈 공간(EMPTY) 중 무작위 좌표 하나를 생성하여 반환합니다.
     * @return std::pair<int, int> {행(Row/Y축), 열(Col/X축)} 구조의 좌표 쌍
     */
    std::pair<int, int> getRandomEmptyPosition() const;

    /**
     * @brief 아이템 매니저로부터 전달받은 검증된 좌표에 아이템 타일을 즉시 배치합니다.
     * @param r 행(Row/Y축)
     * @param c 열(Col/X축)
     * @param item_type GROWTH(5), POISON(6), SHIELD(8)
     */
    void setItem(int r, int c, int item_type);

    /**
     * @brief 아이템을 획득했거나 소멸 시간이 지나 해당 좌표를 다시 빈 공간(EMPTY)으로 초기화합니다.
     * @param r 행(Row/Y축)
     * @param c 열(Col/X축)
     */
    void clearItem(int r, int c);
};
