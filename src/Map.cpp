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

// [성능 최적화] 이전 프레임의 맵 상태를 기록하여 변경된 부분만 그리기 위한 버퍼
static int prev_map[100][100];
static bool force_redraw = true; // 맵이 처음 로드되거나 변경되었을 때 전체 화면을 새로고침하기 위한 플래그

Map::Map(int map_num) : map_num(map_num){
    width = MAP_SIZES[map_num][0];
    height = MAP_SIZES[map_num][1];
    loadMapFile();
}

/**
 * @brief 맵 배열을 전면 초기화하는 함수 (\n 이스케이프 오타 수정 완료)
 */
void Map::mapClear(){
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
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
    // 스테이지가 바뀌었거나 게임이 처음 시작될 때 화면 전체를 완전히 밀고 재설정
    if (force_redraw) {
        clear(); // ncurses 전체 화면 지우기
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                prev_map[i][j] = -1; // 버퍼를 비현실적인 값으로 초기화하여 첫 프레임에 다 그려지도록 유도
            }
        }
        force_redraw = false;
    }

    // 변경된 좌표만 콕 집어서 그리기 (출력 병목 및 프리징 제거 핵심)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            
            // 현재 맵 상태가 이전 프레임과 다를 때만 렌더링 수행
            if (map[i][j] != prev_map[i][j]) {
                move(i, j * 2); // ncurses 커서를 해당 타일 좌표로 정확히 이동 (문자공백 출력이므로 j * 2)
                
                switch (TileType(map[i][j])) {
                    case EMPTY:
                        printw(". "); // 빈 공간
                        break;
                    case WALL:
                        printw("# "); // 일반 벽
                        break;
                    case IMMUNE_WALL:
                        printw("X "); // 관문 불가능 벽
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
                        printw("O "); // 게이트
                        break;
                    default:
                        printw("? "); 
                        break;
                }
                // 화면에 그려진 최신 상태를 버퍼에 기록
                prev_map[i][j] = map[i][j];
            }
        }
    }
    refresh(); // ncurses 가상 화면 반영
    return true;
}

/**
 * @brief 맵 내부의 빈 공간(EMPTY) 중 무작위 좌표 하나를 샘플링하여 반환하는 함수
 * @return std::pair<int, int> {행(row/x), 열(col/y)} 구조의 빈 공간 좌표
 */
std::pair<int, int> Map::getRandomEmptyPosition() {
    int r, c;
    do {
        // 현재 로드된 맵의 높이와 너비 제한 내에서 랜덤 좌표 추출
        r = rand() % height;
        c = rand() % width;
    } while (map[r][c] != 0); // 0(EMPTY) 타일이 나올 때까지 반복 수행
    
    return {r, c};
}

/**
 * @brief 이미 검증된 좌표에 아이템을 즉시 배치하는 함수 (중복 조건문 제거로 최적화)
 * @param r 행(Row)
 * @param c 열(Col)
 * @param item_type 5(GROWTH) 또는 6(POISON)
 */
void Map::setItem(int r, int c, int item_type) {
    if (r >= 0 && r < height && c >= 0 && c < width) {
        map[r][c] = item_type;
    }
}

/**
 * @brief 아이템 획득 및 만료 시 해당 좌표를 빈 공간으로 즉시 되돌리는 함수 (중복 조건문 제거)
 */
void Map::clearItem(int r, int c) {
    if (r >= 0 && r < height && c >= 0 && c < width) {
        map[r][c] = 0; // EMPTY
    }
}
message.txt
