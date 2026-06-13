#pragma once
#include <utility>
#include <vector>
#include "Snake.hpp"
#include "ScoreBoard.hpp"
#include "Map.hpp"
#include <ncurses.h>
#include <iostream>
#include <chrono>

enum class ItemType
{
    Growth = 5,
    Poison = 6,
    Shield = 8   
};

struct ItemInfo
{
    std::pair<int,int> position;//아이템 위치
    ItemType type;
    std::chrono::steady_clock::time_point spawn_time;// 머무는 시간
};


class Item
{
private:
    const int max_num = 3; //조건 아이템수는 최대 3
    std::vector<ItemInfo> active_items;

public:
    Item();
    ~Item();

    void CreateItem(Map& map); //아이템 생성
    void removeExpiredItems(Map& map); // 10초가 지난 아이템을 맵과 리스트에서 삭제
    void clearItem(int x, int y); // 아이템 리스트에서 삭제

    const std::vector<ItemInfo>& getItemsInfo() const { return active_items; } //아이템 정보 get함수
};
