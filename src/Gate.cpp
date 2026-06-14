/**
 * @file Gate.cpp
 * @brief Gate 클래스 - 벽 위 게이트 쌍 생성/제거, 진입 시 반대편 게이트로의 진출 좌표·방향 계산
 */
#include "Gate.hpp"
#include <cstdlib>
#include <vector>

Gate::Gate(int (*p)[MAP_SIZE], int height, int width)
    : arr(p), map_height(height), map_width(width),
      gate_a({-1,-1}), gate_b({-1,-1}), is_active(false) {}


/**
 * @brief Wall 타일 좌표 목록 수집
 */
std::vector<std::pair<int,int>> Gate::collectWalls() const {
    std::vector<std::pair<int,int>> walls;
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (arr[i][j] == WALL) {
                walls.push_back({i, j});
            }
        }
    }
    return walls;
}


/**
 * @brief 랜덤한 Wall 위치 두 곳에 게이트 쌍 생성
 */
void Gate::spawn() {
    // 기존 게이트 제거
    if (is_active) remove();

    std::vector<std::pair<int,int>> walls = collectWalls();
    if (walls.size() < 2) return;

    // 랜덤으로 서로 다른 두 위치 선택
    int idx_a = rand() % walls.size();
    int idx_b;
    do {
        idx_b = rand() % walls.size();
    } while (idx_b == idx_a);

    gate_a = walls[idx_a];
    gate_b = walls[idx_b];

    arr[gate_a.first][gate_a.second] = GATE;
    arr[gate_b.first][gate_b.second] = GATE;
    is_active = true;
}


/**
 * @brief 게이트 제거 후 Wall로 복원
 */
void Gate::remove() {
    if (!is_active) return;

    arr[gate_a.first][gate_a.second] = WALL;
    arr[gate_b.first][gate_b.second] = WALL;
    gate_a = {-1, -1};
    gate_b = {-1, -1};
    is_active = false;
}


/**
 * @brief 벽의 위치에 따라 기본 진출 방향 결정
 * 상단 벽 → 아래(2), 하단 벽 → 위(0)
 * 좌측 벽 → 오른쪽(1), 우측 벽 → 왼쪽(3)
 */
int Gate::getWallDirection(std::pair<int,int> pos) const {
    int r = pos.first;
    int c = pos.second;

    if (r == 0)              return 2; // 상단 벽 → 아래
    if (r == map_height - 1) return 0; // 하단 벽 → 위
    if (c == 0)              return 1; // 좌측 벽 → 오른쪽
    if (c == map_width - 1)  return 3; // 우측 벽 → 왼쪽

    // 내부 벽: 빈 공간 쪽으로 진출
    // 상하좌우 중 EMPTY인 방향 반환
    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, 1, 0, -1};
    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d];
        int nc = c + dc[d];
        if (nr >= 0 && nr < map_height && nc >= 0 && nc < map_width) {
            if (arr[nr][nc] == EMPTY) return d;
        }
    }
    return 2; // fallback
}


/**
 * @brief 가장자리(코너) 여부 확인
 */
bool Gate::isCorner(std::pair<int,int> pos) const {
    int r = pos.first;
    int c = pos.second;
    bool on_top_bottom = (r == 0 || r == map_height - 1);
    bool on_left_right = (c == 0 || c == map_width - 1);
    return on_top_bottom && on_left_right;
}


/**
 * @brief 코너 게이트의 진출 방향 결정 (우선순위 적용)
 * 우선순위: 진입방향 일치 → 시계방향 → 역시계방향 → 반대방향
 */
int Gate::getCornerExitDirection(std::pair<int,int> exit_gate, int entry_dir) const {
    // 시계 방향: 상(0)->우(1)->하(2)->좌(3)->상(0)
    int clockwise     = (entry_dir + 1) % 4;
    int counter_clock = (entry_dir + 3) % 4;
    int opposite      = (entry_dir + 2) % 4;

    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, 1, 0, -1};
    int r = exit_gate.first;
    int c = exit_gate.second;

    // 우선순위 순서대로 이동 가능한 방향 반환
    for (int candidate : {entry_dir, clockwise, counter_clock, opposite}) {
        int nr = r + dr[candidate];
        int nc = c + dc[candidate];
        if (nr >= 0 && nr < map_height && nc >= 0 && nc < map_width) {
            if (arr[nr][nc] != WALL && arr[nr][nc] != IMMUNE_WALL) {
                return candidate;
            }
        }
    }
    return opposite; // fallback
}


/**
 * @brief Snake가 게이트 진입 시 진출 좌표와 방향 계산
 * @param entry_pos 진입한 게이트 좌표
 * @param entry_dir 진입 방향 (0:상 1:우 2:하 3:좌)
 * @param exit_pos  진출 게이트 좌표 (out)
 * @param exit_dir  진출 방향 (out)
 */
void Gate::getExitInfo(std::pair<int,int> entry_pos, int entry_dir,
                       std::pair<int,int>& exit_pos, int& exit_dir) const {
    // 진출 게이트는 반대편
    if (entry_pos == gate_a) exit_pos = gate_b;
    else                     exit_pos = gate_a;

    // 코너 여부에 따라 진출 방향 결정
    if (isCorner(exit_pos)) {
        exit_dir = getCornerExitDirection(exit_pos, entry_dir);
    } else {
        exit_dir = getWallDirection(exit_pos);
    }
}