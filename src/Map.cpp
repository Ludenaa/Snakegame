/**
 * @file Map.cpp
 * @brief 게임 맵 데이터 로드, 더블 버퍼링 기반의 최적화 렌더링 및 실시간 오브젝트 배치 공간 연산 구현
 */

#include "Map.hpp"
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> // rand() 사용을 위해 추가
#include <utility> // std::pair 사용을 위해 추가
#include "Config.hpp"  // ⭐ 추가

Map::Map(const GameConfig& config) {
    map_num = config.map_num;
    width = config.map_size.width;    // ⭐ Config의 가로 크기 참조
    height = config.map_size.height;  // ⭐ Config의 세로 크기 참조
    force_redraw = true;
    loadMapFile();
}

/**
 * @brief 맵 배열을 전면 초기화하는 함수
 */
void Map::mapClear(){
    for(int i=0; i<MAP_SIZE; i++){
        for(int j=0; j<MAP_SIZE; j++){
            map[i][j] = 0;
        }
    }
}

bool Map::loadMapFile(){
    // 맵 txt 파일을 읽어와 맵에 대입
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
    
    // 맵 파일이 새로 로드되면 이전 버퍼를 초기화하고 강제 전체 렌더링을 유도합니다.
    force_redraw = true; 
    return true;
}

/**
 * @brief 누수 및 프리징 해결을 위한 최적화된 렌더링 로직 (Double Buffering 기법 적용)
 */
bool Map::render(){
    if (force_redraw) {
        clear(); // ncurses 전체 화면 지우기
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                prev_map[i][j] = -1; // 버퍼 초기화
            }
        }
        force_redraw = false;
    }

    // i = 행(Row/Y축), j = 열(Col/X축) 순서로 순회
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            
            // 현재 맵 상태가 이전 프레임과 다를 때만 해당 칸을 찾아가서 갱신
            if (map[i][j] != prev_map[i][j]) {
                move(i, j * 2); // ncurses는 move(y, x) 형식을 취하므로 move(행, 열*2)가 정확합니다.
                
                switch (TileType(map[i][j])) {
                    case EMPTY:       printw(". "); break;
                    case WALL:        printw("# "); break;
                    case IMMUNE_WALL: printw("X "); break;
                    case SNAKE_HEAD:  printw("H "); break;
                    case SNAKE_BODY:  printw("B "); break;
                    case GROWTH:      printw("G "); break;
                    case POISON:      printw("P "); break;
                    case GATE:        printw("O "); break;
                    case SHIELD:      printw("S "); break; // ⭐ 실드 아이템 스폰 시 화면에 'S ' 출력 추가
                    default:          printw("? "); break;
                }
                prev_map[i][j] = map[i][j];
            }
        }
    }
    refresh(); 
    return true;
}

/**
 * @brief 맵 내부의 빈 공간(EMPTY) 중 무작위 좌표 하나를 샘플링하여 반환하는 함수
 * @return std::pair<int, int> {행(Row/Y축), 열(Col/X축)} 구조의 빈 공간 좌표
 */
std::pair<int, int> Map::getRandomEmptyPosition() const{
    int r, c;
    do {
        // 행(r)은 height(세로) 범위 내에서, 열(c)은 width(가로) 범위 내에서 무작위 선택
        r = rand() % height;
        c = rand() % width;
    } while (map[r][c] != 0); // 0(EMPTY) 타일이 나올 때까지 반복 수행
    
    return {r, c}; // {행, 열} 순서로 반환하여 Item.cpp의 empty_block에 대입됨
}

/**
 * @brief 이미 검증된 좌표에 아이템을 즉시 배치하는 함수
 * @param r 행(Row)
 * @param c 열(Col)
 * @param item_type 5(GROWTH), 6(POISON), 8(SHIELD)
 */
void Map::setItem(int r, int c, int item_type) {
    if (r >= 0 && r < height && c >= 0 && c < width) {
        map[r][c] = item_type;
    }
}

/**
 * @brief 아이템 획득 및 만료 시 해당 좌표를 빈 공간으로 즉시 되돌리는 함수
 */
void Map::clearItem(int r, int c) {
    if (r >= 0 && r < height && c >= 0 && c < width) {
        map[r][c] = 0; // EMPTY
    }
}
