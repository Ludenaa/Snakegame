#include "Map.hpp"
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Map::Map(int map_num) : map_num(map_num){
    width = MAP_SIZES[map_num][0];
    height = MAP_SIZES[map_num][1];
    loadMapFile();
}

void Map::mapClear(){
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            map[i][j] = 0;
        }
    }
}

bool Map::loadMapFile(){
    //맵 txt 파일을 읽어와 맵에 대입
    std::string filename = "../data/map" + std::to_string(map_num) + ".txt";
    std::ifstream openFile(filename);
    if (!openFile.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return false;
    }
    
    mapClear();

    std::string line;
    int i=0;
    while (std::getline(openFile, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        int value;

        int j=0;
        while (ss >> value) {
            map[i][j++] = static_cast<int>(value);
        }
        i++;
    }

    openFile.close();
    return true;
}

bool Map::render(){
    // 화면에 맵 그리기
    move(0, 0); //맵 출력 지점 이동

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            switch (TileType(map[i][j])) {
                case EMPTY:
                    printw(". "); // 빈 공간
                    break;
                case WALL:
                    printw("# "); // 일반 벽
                    break;
                case IMMUNE_WALL:
                    printw("X "); // Immune Wall (관문이 안 생기는 벽)
                    break;
                case SNAKE_HEAD:
                    printw("H "); // 뱀 머리
                    break;
                case SNAKE_BODY:
                    printw("B "); // 뱀 몸통
                    break;
                case GROWTH:
                    printw("G "); // 그로스 아이템
                    break;
                case POISON:
                    printw("P "); // 포이즌 아이템
                    break;
                case GATE:
                    printw("  "); // 게이트 (공백 등으로 표시 가능)
                    break;
                default:
                    printw("? "); // 예외 처리
                    break;
            }
        }
        // 한 행의 출력이 끝나면 줄바꿈
        printw("\n");
    }
    refresh();

    return true;
}