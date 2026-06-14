cmake_minimum_required(VERSION 3.10)
project(SnakeGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)
set(SOURCES
    src/main.cpp
    src/Config.cpp
    src/Map.cpp
    src/Snake.cpp
    src/Item.cpp
    src/Gate.cpp
    src/ScoreBoard.cpp
)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS} ${CURSES_INCLUDE_DIR})
find_library(NCURSESW_LIB NAMES ncursesw)


if(NOT NCURSESW_LIB)
    message(FATAL_ERROR
        "libncursesw(와이드 ncurses)를 찾을 수 없습니다.\n"
        "  Ubuntu/Debian : sudo apt install libncursesw5-dev\n"
        "  Fedora/RHEL   : sudo dnf install ncurses-devel\n"
        "설치 후 build 폴더를 비우고 다시 빌드하세요.")
endif()

# 실행및 링킹
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${NCURSESW_LIB})