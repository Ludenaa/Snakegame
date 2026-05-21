#include "Item.hpp"

Item::Item() {}
Item::~Item(){}

void Item::CreateItem(const Map& map){
    if(active_items.size() >= max_num) return; // 조건 : 동시에 최대 3개 존재

    ItemInfo item;
    int x,y;
    do{ //빈좌표 받기
        x = rand() % 20 + 1;
        y = rand() % 20 + 1;
    } while (!map.is_empty(x,y));

    item.type = (rand() % 2 == 1) ? ItemType::Growth : ItemType::Poison; 
    // 1이면 Growth 0이면 Poison

    item.position = {x,y}; //아이템 위치 저장 x,y순서
    item.time =  100 // 임시 시간 지정

    active_items.push_back(item);
}

bool Item::TakeItem(const Snake& snake, const Map& map){
    std::pair<int, int> snake_head = map.get(); // 정보받아옴
    
    int snake_x = snake_head.first;
    int snake_y = snake_head.second;
    for(int i = 0; i < active_items.size(); i++){ // 아이템이랑 비교
        int item_x = active_items[i].position.first;
        int item_y = active_items[i].position.second;

        if (snake_x == item_x && snake_y == item_y) {item_idx = i; return true;}
    }
    return false;
}

void Item::ApplyItem(Snake& snake, ScoreBoard& score){
    ItemInfo eaten_item = active_items[item_idx]; //
    switch (eaten_item.type)
    {
        case ItemType::Growth:
            snake.grow(eaten_item.position.first, eaten_item.position.second);
            // score.add_growth();
            score.growth_cnt++;
            break;
        case ItemType::Poison:
            snake.decrease();
            // score.add_poison();
            score.poison_cnt++;
            break;
    }
    active_items.erase(active_items.begin() + item_idx); //리스트에서 제거
    item_idx = -1; // 초기화
}