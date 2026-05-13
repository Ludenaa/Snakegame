#include "Snake.hpp"

/**
 * @brief 스네이크 생성자
 * @param start_y 머리가 시작할 Y 좌표
 * @param start_x 머리가 시작할 X 좌표
 * @param initial_length 초기 뱀의 길이 (기본값 3)
 */
Snake::Snake(int start_y, int start_x, int initial_length) {
    // 초기 방향은오른쪽(RIGHT)으로 설정
    dir = Direction::RIGHT;

    // 머리부터 시작해서 왼쪽으로 몸통을 이어 붙임
    for (int i = 0; i < initial_length; i++) {
        body.push_back({start_y, start_x - i});
    }
}

Snake::~Snake() {}

const std::vector<Position>& Snake::get_body() const { return body; }
Position Snake::get_head() const { return body.front(); }
Direction Snake::get_direction() const { return dir; }

/**
 * @brief 뱀의 이동 방향 변경 (진행 반대 방향으로의 역주행 방지)
 * / 1 2 3 4 순으로 상 하 좌 우 순서
 */
void Snake::change_direction(int dir) {
    if (dir == Direction::UP && new_dir == Direction::DOWN) return;
    if (dir == Direction::DOWN && new_dir == Direction::UP) return;
    if (dir == Direction::LEFT && new_dir == Direction::RIGHT) return;
    if (dir == Direction::RIGHT && new_dir == Direction::LEFT) return;
    
    dir = new_dir;
}

/**
 * @brief 뱀을 한 칸 이동시키는 핵심 로직
 * @param grow true일 경우 꼬리를 자르지 않아 길이가 1 늘어남 (Growth 아이템 효과)
 */
void Snake::move(bool grow) {
    Position next_head = get_head();

    // 현재 방향에 따라 다음 머리 좌표 계산
    switch (dir) {
        case Direction::UP:    next_head.y--; break;
        case Direction::DOWN:  next_head.y++; break;
        case Direction::LEFT:  next_head.x--; break;
        case Direction::RIGHT: next_head.x++; break;
    }

    // 새로운 머리를 맨 앞에 추가
    body.insert(body.begin(), next_head);

    // 몸이 자라나는 상태(grow == true)가 아니라면 맨 뒤의 꼬리를 제거
    if (!grow) {
        body.pop_back();
    }
}

/**
 * @brief 뱀이 자신의 몸통과 충돌했는지 검사 (머리 좌표가 몸통 좌표와 겹치는지)
 */
bool Snake::check_self_collision() const {
    Position head = get_head();
    // body[0]은 머리이므로 body[1]부터 끝까지 검사
    for (size_t i = 1; i < body.size(); i++) {
        if (head == body[i]) {
            return true; // 충돌 발생
        }
    }
    return false;
}