/**
 * @file Map.hpp
 * @brief 스네이크 게임의 맵 데이터 구조 정의 및 오브젝트 배치/조회를 위한 인터페이스 제공
 */

#pragma once
#define MAP_HPP

#include <vector>
#include <string>
#include <utility> // std::pair 구조체 사용을 위해 추가

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
    const int MAP_SIZES[5][2] = {{0, 0}, {21, 21}, {25, 25}, {31, 31}, {0, 0}}; // 맵 번호에 따른 (높이, 너비) 매핑

    int width;
    int height;
    int map_num;

    bool loadMapFile();

public:
    Map(int map_num);
    int map[100][100];

    void mapClear();
    bool render() const;

    int getWidth() const;
    int getHight() const;
    
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
