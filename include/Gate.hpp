/**
 * @file Gate.hpp
 * @brief 맵의 벽 위에 생성되는 게이트(Gate) 쌍의 생성/제거 및 진출 좌표/방향 계산 인터페이스 선언
 */
#pragma once

#include <utility>
#include <vector>
#include "Map.hpp"   // MAP_SIZE, TileType 정의 사용

/**
 * @brief Gate 클래스
 * - Wall(1) 위치에만 생성 가능, Immune Wall(2) 불가
 * - 항상 한 쌍(gate_a, gate_b)으로 존재
 * - 맵 배열에서 Gate 값은 7
 */
class Gate {
private:
    // 읽기 전용 맵 포인터 타입.
    // 요소 타입이 const int 이므로, 이 포인터를 통해서는 맵을 수정할 수 없다.
    using ConstMapPtr = const int (*)[MAP_SIZE];

    int (*arr)[MAP_SIZE];   // Map의 2차원 배열 포인터 (쓰기 경로: spawn/remove)
    int map_height;
    int map_width;

    std::pair<int,int> gate_a;  // 게이트 A 좌표
    std::pair<int,int> gate_b;  // 게이트 B 좌표
    bool is_active;             // 게이트 활성화 여부

    // const 멤버 함수에서 맵을 '읽기 전용'으로만 다루기 위한 const 뷰.
    // raw 포인터 멤버는 const가 대상까지 전파되지 않으므로(예: const 함수
    // 안에서도 arr[i][j] 수정이 그대로 컴파일됨), 모든 읽기 동작은 이 뷰를
    // 거치게 한다. 반환 타입의 요소가 const 이므로 const 함수 내에서의 맵
    // 수정이 컴파일 단계에서 차단된다.
    ConstMapPtr constArr() const { return arr; }

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
};