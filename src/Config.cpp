/**
 * @file Config.cpp
 * @brief 게임 난이도 및 난이도별 물리적 맵 규격 매핑 정의
 */
#include "Config.hpp"

```
GameConfig makeConfig(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:
            // Easy : 1번 맵, 루프 간격 200ms, 미션 스코어 조건, 맵 사이즈 21x21
            // 목표 : 튜토리얼 처럼 느낄 수 있는 간단한 난이도
            return { d, 1, 200, {5, 2, 2, 2}, {21, 21} };
            
        case Difficulty::Normal:
            // Normal : 2번 맵, 루프 간격 150ms, 미션 스코어 조건, 맵 사이즈 25x25
            // 목표 : 간단한 도전, 길이의 한계 체험
            return { d, 2, 170, {15, 1, 1, 15}, {25, 25} };
            
        case Difficulty::Hard:
            // Hard : 3번 맵, 루프 간격 100ms, 미션 스코어 조건, 맵 사이즈 31x31
            // 목표 : 어려운 도전, 독을 최대한 많이 맛보기
            return { d, 3, 130, {5, 5, 10, 5}, {31, 31} };
        case Difficulty::Extreme:
            // Extreme : 4번 맵, 루프 간격 80ms, 미션 스코어 조건, 맵 사이즈 31x31
            // 목표 : 챌린저, 미션 난이도 및 속도 전반적인 상향
            return { d, 4, 80, {25, 25, 25, 25}, {35, 35} };
            
        default:
            // 예외 처리 구조 (디폴트 Easy 지정)
            return { d, 1, 200, {1, 1, 1, 1}, {21, 21} };
    }
}
```