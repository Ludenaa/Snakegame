/**
 * @file Config.cpp
 * @brief 게임 난이도 및 난이도별 물리적 맵 규격 매핑 정의
 */
#include "Config.hpp"

GameConfig makeConfig(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:
            // Easy : 1번 맵, 루프 간격 200ms, 미션 스코어 조건, 맵 사이즈 21x21
            return { d, 1, 200, {1, 0,}, {21, 21} };
            
        case Difficulty::Normal:
            // Normal : 2번 맵, 루프 간격 150ms, 미션 스코어 조건, 맵 사이즈 25x25
            return { d, 2, 150, {1, 0,}, {25, 25} };
            
        case Difficulty::Hard:
            // Hard : 3번 맵, 루프 간격 100ms, 미션 스코어 조건, 맵 사이즈 31x31
            return { d, 3, 100, {1, 0,}, {31, 31} };
        case Difficulty::Extreme:
            // Extreme : 4번 맵, 루프 간격 80ms, 미션 스코어 조건, 맵 사이즈 31x31
            return { d, 4, 80, {4, 0,}, {35, 35} };
            
        default:
            // 예외 처리 구조 (디폴트 Easy 지정)
            return { d, 1, 200, {1, 1, 1, 1}, {21, 21} };
    }
}

// switch (d) {
//         case Difficulty::Easy:
//             // Easy : 1번 맵, 루프 간격 200ms, 미션 스코어 조건, 맵 사이즈 21x21
//             return { d, 1, 200, {10, 5, 2, 1}, {21, 21} };
            
//         case Difficulty::Normal:
//             // Normal : 2번 맵, 루프 간격 150ms, 미션 스코어 조건, 맵 사이즈 25x25
//             return { d, 2, 150, {15, 8, 3, 5}, {25, 25} };
            
//         case Difficulty::Hard:
//             // Hard : 3번 맵, 루프 간격 100ms, 미션 스코어 조건, 맵 사이즈 31x31
//             return { d, 3, 100, {20, 10, 5, 10}, {31, 31} };

        // case Difficulty::Extreme:
            // Extreme : 4번 맵, 루프 간격 80ms, 미션 스코어 조건, 맵 사이즈 31x31
        //     return { d, 4, 80, {25, 10, 10, 20}, {35, 35} };
            

//         default:
//             // 예외 처리 구조 (디폴트 Easy 지정)
//             return { d, 1, 200, {10, 5, 2, 1}, {21, 21} };
//     }
