#include "Snake.hpp"

/**
 * @brief x : 꼬리의 행 좌표, y = 꼬리의 열 좌표, snakesize : 뱀의 길이, int (*p)[MAP_SIZE] : 2차원 배열 포인터, firstdir : 초기 방향
 */
Snake::Snake(int x, int y, int snakesize, int firstdir, int (*p)[MAP_SIZE]) {
    dir = firstdir;
    arr = p;
    dx = x;
    dy = y;

    // 꼬리부터 시작해서 진행방향쪽으로 머리까지 이어붙임
    for (int i = 0; i < snakesize; i++) {
        body.push_back({dx,dy});
        arr[dx][dy] = 4;
        dx += Dirction[dir][0];
        dy += Dirction[dir][1];
    }
    arr[body.back().first][body.back().second] = 3;
}

Snake::~Snake() {}

/**
 * @brief 뱀의 이동 방향 변경 
 * 0 1 2 3 각각 상 우 좌 하
 * 반대 방향을 누르면 실패하는 로직은 move 에서 구현
 */
void Snake::change_direction(int nextdir) {
    dir = nextdir;
}

/**
 * @brief 뱀을 한 칸 이동시킴
 * 이동이 가능하면 true 를, 이동이 불가하면 false 를 return
 */
bool Snake::move() {
    dx = body.back().first + Dirction[dir][0];
    dy = body.back().second + Dirction[dir][1];

    //배열 크기를 벗어날 경우 false
    if(dx < 0 || dx >= MAP_SIZE || dy < 0 || dy >= MAP_SIZE) return false;

    // 이동 방향에 0: 빈공간 일 경우 꼬리 한 칸 제거, 앞으로 한 칸 이동
    else if(arr[dx][dy] == 0){
        grow(dx,dy);
        decrease();
    }

    // 이동 방향에 1: 벽, 2: Immune Wall, 3: Snake 있을 경우 false
    else if(arr[dx][dy] == 1 || arr[dx][dy] == 2 || arr[dx][dy] == 3) return false;

    //growth item 을 먹었을 경우
    else if(arr[dx][dy] == 11){
        grow(dx,dy);
    }
    //poison item 을 먹었을 경우
    else if(arr[dx][dy] == 22){
        grow(dx,dy);
        decrease();
        decrease();
    }
    //portal 에 들어갔을 경우
    else if(arr[dx][dy] == 33){
        ingate = {dx,dy};
        outgate = {999,999};

        /* 미구현
        //진출 게이트가 가장자리에 있을 때
        if(outgate.first == 0 || outgate.first == MAP_SIZE-1 || outgate.second == 0 || outgate.second == MAP_SIZE-1){
            if(outgate.first == 0) change_direction(2);
            else if(outgate.second == MAP_SIZE-1)change_direction(3);
            else if(outgate.first == MAP_SIZE-1)change_direction(0);
            else change_direction(1);
            grow(outgate.first+ Dirction[dir][0],outgate.second+ Dirction[dir][1]);
        }
        //진출 방향이 좌 우 일경우 (상하 방향이 벽으로 막혀있을 경우)
        else if(arr[outgate.first+1][outgate.second] == 1 && arr[outgate.first-1][outgate.second] == 1){

        }
        */
    }

    //뱀의 길이가 3보다 작을 경우 경우 false
    if(body.size() < 3) return false;

    return true;
}

/**
 * @brief x,y 좌표로 뱀을 1 성장시킴
 */
void Snake::grow(int x, int y){
    arr[body.back().first][body.back().second] = 4;
    body.push_back({x,y});
    arr[x][y] = 3;
}

/**
 * @brief 뱀의 꼬리 부분을 1 감소시킴
 */
void Snake::decrease(){
    dx = body.front().first,
    dy = body.front().second;
    body.pop_front();
    arr[dx][dy] = 0;
}