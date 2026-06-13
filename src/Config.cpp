/**
 * @file Config.cpp
 * @brief 게임 난이도 설정 정의
 */
#include "Config.hpp"

GameConfig makeConfig(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:
            return { d, 1, 250, {10, 5, 2, 1} };
        case Difficulty::Hard:
            return { d, 3, 120, {20, 10, 5, 10} };
        case Difficulty::Normal:
        default:
            return { d, 2, 180, {15, 8, 3, 5} };
    }
}