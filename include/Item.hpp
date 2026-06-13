#pragma once
#include <utility>
#include <vector>
#include "Snake.hpp"
#include "ScoreBoard.hpp"
#include "Map.hpp"
#include <ncurses.h>
#include <iostream>

enum class ItemType
{
    Growth,
    Poison,
    Shield   
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
    int item_idx;

public:
    Item();
    ~Item();

    void CreateItem(Map& map); //아이템 생성

    bool TakeItem(const Snake& snake, const Map& map); //아이템 먹었는지 확인

    void ApplyItem(Snake& snake, ScoreBoard& score); //아이템 적용

    void deleteItem(); //기존 아이템 삭제

    const std::vector<ItemInfo>& getItemsInfo() const { return active_items; } 
};
