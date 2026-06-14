/**
 * @file Gate.hpp
 * @brief 맵의 벽 위에 생성되는 게이트(Gate) 쌍의 생성/제거 및 진출 좌표/방향 계산 인터페이스 선언
 */
#pragma once

#include <utility>
#include "Map.hpp"   // MAP_SIZE, TileType 정의 사용

/**
 * @brief Gate 클래스
 * - Wall(1) 위치에만 생성 가능, Immune Wall(2) 불가
 * - 항상 한 쌍(gate_a, gate_b)으로 존재
 * - 맵 배열에서 Gate 값은 7
 */
class Gate {
private:
    int (*arr)[MAP_SIZE];   // Map의 2차원 배열 포인터
    int map_height;
    int map_width;

    std::pair<int,int> gate_a;  // 게이트 A 좌표
    std::pair<int,int> gate_b;  // 게이트 B 좌표
    bool is_active;             // 게이트 활성화 여부

    // 벽의 위치로 기본 진출 방향 결정 (0:상 1:우 2:하 3:좌)
    int getWallDirection(std::pair<int,int> pos) const;

    // 가장자리(코너) 여부 확인
    bool isCorner(std::pair<int,int> pos) const;

    // 코너일 때 우선순위에 따라 진출 방향 결정
    int getCornerExitDirection(std::pair<int,int> exit_gate, int entry_dir) const;

    // Wall 타일 목록 수집
    std::vector<std::pair<int,int>> collectWalls() const;

public:
    Gate(int (*p)[MAP_SIZE], int height, int width);
    ~Gate() = default;

    // 랜덤한 Wall 위치에 게이트 쌍 생성
    void spawn();

    // 게이트 제거 (맵에서 Wall로 복원)
    void remove();

    // Snake가 게이트에 진입했을 때 진출 좌표와 방향 계산
    // entry_pos: 진입한 게이트 좌표, entry_dir: 진입 방향
    // exit_pos: 진출 좌표(out), exit_dir: 진출 방향(out)
    void getExitInfo(std::pair<int,int> entry_pos, int entry_dir,
                     std::pair<int,int>& exit_pos, int& exit_dir) const;

    bool getIsActive() const { return is_active; }
    std::pair<int,int> getGateA() const { return gate_a; }
    std::pair<int,int> getGateB() const { return gate_b; }
};