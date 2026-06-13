#include "Snake.hpp"
#include "Map.hpp"

/**
 * @brief x : 꼬리의 행 좌표, y : 꼬리의 열 좌표
 *        snakesize : 뱀의 초기 길이, firstdir : 초기 방향
 *        p : 맵 배열 포인터, g : Gate 포인터
 */
Snake::Snake(int x, int y, int snakesize, int firstdir, int (*p)[MAP_SIZE], Gate* g, ScoreBoard* sb) {
    dir = firstdir;
    arr = p;
    gate = g;
    dx = x;
    dy = y;
    shield = false;
    scoreBoard = sb;

    // 꼬리부터 시작해서 진행방향쪽으로 머리까지 이어붙임
    for (int i = 0; i < snakesize; i++) {
        body.push_back({dx, dy});
        arr[dx][dy] = SNAKE_BODY;
        dx += Dirction[dir][0];
        dy += Dirction[dir][1];
    }
    arr[body.back().first][body.back().second] = SNAKE_HEAD;
}

Snake::~Snake() {}


/**
 * @brief 뱀의 이동 방향 변경
 *        반대 방향 입력 시 무시
 */
void Snake::changeDirection(int nextdir) {
    // 반대 방향: 현재 방향과 합이 2(상-하) 또는 합이 4(좌-우)
    int opposite = (dir + 2) % 4;
    if (nextdir == opposite) return;
    dir = nextdir;
}


/**
 * @brief 뱀을 한 칸 이동
 * @return 이동 가능하면 true, 게임 오버 조건이면 false
 */
bool Snake::move() {
    dx = body.back().first  + Dirction[dir][0];
    dy = body.back().second + Dirction[dir][1];

    // 배열 범위 초과
    if (dx < 0 || dx >= MAP_SIZE || dy < 0 || dy >= MAP_SIZE) return false;

    switch (TileType(arr[dx][dy])) {

        case EMPTY:
            grow(dx, dy);
            decrease();
            break;

        case WALL:
        case IMMUNE_WALL:
        case SNAKE_HEAD:
        case SNAKE_BODY:
            return false; //겜 오버

        case GROWTH:
            grow(dx, dy);
            scoreBoard-> addGrowth();
            // sb에 growth 전달 -> sb.addGrowth()
            break;

        case POISON:
            grow(dx, dy);
            decrease();
            if(shield) shield = false;
            else decrease(); // poison 먹으면 길이 2칸 줄어듬, shield 있으면 1칸만 줄어듬
            scoreBoard->addPoison();
            // sb에 posion 전달 -> sb.addPoison()
            break;

        case GATE: {
            std::pair<int,int> exit_pos;
            int exit_dir;
            gate->getExitInfo({dx, dy}, dir, exit_pos, exit_dir);

            //github 이슈#3
            // changeDirection(exit_dir);
            dir = exit_dir;

            // 게이트 위치에서 진출 방향으로 한 칸 이동한 위치
            // 게이트 자체에서 구현하려고 했는데 여기가 더 깔끔해서 여기서 +1 할게
            int next_r = exit_pos.first  + Dirction[exit_dir][0];
            int next_c = exit_pos.second + Dirction[exit_dir][1];

            grow(next_r, next_c);
            decrease();
            scoreBoard->addGateCnt();
            break;
        }

        case SHIELD: 
            shield = true;
            grow(dx, dy);
            decrease();
            break;
        

        default:
            break;
    }

    // 뱀의 길이가 3 미만이면 게임 오버
    if (body.size() < 3) return false;

    return true;
}


/**
 * @brief (x, y) 위치로 뱀 머리를 1칸 성장
 */
void Snake::grow(int x, int y) {
    arr[body.back().first][body.back().second] = SNAKE_BODY;
    body.push_back({x, y});
    arr[x][y] = SNAKE_HEAD;
}


/**
 * @brief 뱀의 꼬리를 1칸 감소
 */
void Snake::decrease() {
    dx = body.front().first;
    dy = body.front().second;
    body.pop_front();
    arr[dx][dy] = EMPTY; // 이미 뱀으로 덮어서 게이트를 지난 경우 문제가 생김, 수정해야함
}