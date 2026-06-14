/**
 * @file Item.cpp
 * @brief 빈 공간에 아이템을 무작위 생성하고, 만료(10초)·획득 시 맵과 목록에서 제거하는 아이템 클래스 구현
 */
#include "Item.hpp"

Item::Item() {}
Item::~Item() {}    

void Item::CreateItem(Map& map, Difficulty level){
    if(active_items.size() >= max_num) return; // 조건 : 동시에 최대 3개 존재

    ItemInfo item;
    std::pair<int, int> empty_block = map.getRandomEmptyPosition();
    
    if (level == Difficulty::Easy || level == Difficulty::Normal) {
        // 1단계, 2단계 Growth, Poison, Shield가 1:1:1 비율로 생성
        int rand_val = rand() % 3; 
        if(rand_val == 0) item.type = ItemType::Growth;
        else if (rand_val == 1) item.type = ItemType::Poison;
        else item.type = ItemType::Shield;
    } 
    else if (level == Difficulty::Hard) {
        //쉴드x Growth, Poison이 1:1 비율로 생성
        int rand_val = rand() % 2; 
        if(rand_val == 0) item.type = ItemType::Growth;
        else item.type = ItemType::Poison;
    }
    else if (level == Difficulty::Extreme) {
        //쉴드x Growth, Poison 1:2 비율로 생성
        int rand_val = rand() % 3; 
        if(rand_val == 0) {
            item.type = ItemType::Growth;
        } else {
            item.type = ItemType::Poison; 
        }
    }

    item.position = empty_block; //아이템 위치 저장 x,y순서
    item.spawn_time = std::chrono::steady_clock::now();// 생성 시간 지정
    active_items.push_back(item);
    map.setItem(item.position.first, item.position.second, static_cast<int>(item.type));
}

void Item::removeExpiredItems(Map& map) { // 기간이 지난 아이템 맵 삭제 
    auto now = std::chrono::steady_clock::now();
    for (auto it = active_items.begin(); it != active_items.end(); ) {
        int time = std::chrono::duration_cast<std::chrono::seconds>(now - it->spawn_time).count();
        if (time >= 10) {
            map.clearItem(it->position.first, it->position.second);
            it = active_items.erase(it); // 리스트에서 지우기
        } else {
            ++it;
        }
    }
}
void Item::clearItem(int x, int y) { // 먹은 아이템 리스트에서 삭제 
    for (auto it = active_items.begin(); it != active_items.end(); ++it) {
        if (it->position.first == x && it->position.second == y) {
            active_items.erase(it);
            return;
        }
    }
}
