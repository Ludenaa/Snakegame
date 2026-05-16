#pragma once
#define MAP_HPP
#include <vector>
#include <string>

enum TileType {
    EMPTY = 0,
    WALL = 1,
    IMMUNE_WALL = 2,
    SNAKE_HEAD = 3,
    SNAKE_BODY = 4,
    GROWTH = 5,
    POISON = 6,
    GATE = 7
};

class Map {
private:
    const int MAP_SIZES[5][2] = {{0, 0}, {21, 21}};

    int width;
    int height;
    int map_num;

    bool loadMapFile();
public:
    Map(int map_num);
    int map[100][100];

    void mapClear();
    bool render();

    int getWidth();
    int getHight();
};