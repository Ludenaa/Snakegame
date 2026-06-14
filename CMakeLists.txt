# 1. CMake 실행을 위한 최소 버전을 설정합니다. (너무 낮으면 최신 기능을 못 씀)
cmake_minimum_required(VERSION 3.10)

# 2. 프로젝트의 이름과 버전, 사용할 언어(CXX = C++)를 정의합니다.
project(SnakeGame VERSION 1.0.0 LANGUAGES CXX)

# 3. C++ 컴파일 표준을 설정합니다. (여기서는 C++17 사용)
# C++17의 편리한 기능들(auto, 구조화된 바인딩 등)을 쓰기 위함입니다.
set(CMAKE_CXX_STANDARD 17)

# 4. 설정한 표준을 반드시 따르도록 강제합니다. (지원 안 되면 에러 발생)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 5. 최종 결과물인 실행 파일(.exe 혹은 바이너리)이 저장될 경로를 지정합니다.
# ${CMAKE_SOURCE_DIR}는 CMakeLists.txt가 있는 루트 폴더를 의미합니다.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 6. 컴파일러가 헤더 파일(.hpp)을 찾을 위치를 알려줍니다.
# 이제 코드에서 #include "../include/ScoreBoard.hpp" 대신 "ScoreBoard.hpp"로 쓸 수 있습니다.
include_directories(include)

# 7. src 폴더 내에 있는 모든 .cpp 파일을 자동으로 찾아서 SOURCES라는 변수에 저장합니다.
# 파일을 새로 만들 때마다 일일이 이름을 적지 않아도 되어 편리합니다.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 8. 시스템 내에 ncurses 라이브러리가 설치되어 있는지 확인합니다.
# 유니코드(■ 등 와이드 문자) 출력을 위해 ncursesw(와이드 버전)가 반드시 필요합니다.
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

# 9. ncurses의 헤더 파일 경로를 컴파일러에게 알려줍니다.
include_directories(${CURSES_INCLUDE_DIRS} ${CURSES_INCLUDE_DIR})

# 9-1. [중요] 환경/캐시에 따라 find_package가 좁은(narrow) libncurses로 잡혀
#      'undefined reference to waddnwstr' 같은 링크 오류가 날 수 있습니다.
#      와이드 라이브러리(ncursesw)를 명시적으로 찾아 직접 링크합니다.
find_library(NCURSESW_LIB NAMES ncursesw)
if(NOT NCURSESW_LIB)
    message(FATAL_ERROR
        "libncursesw(와이드 ncurses)를 찾을 수 없습니다.\n"
        "  Ubuntu/Debian : sudo apt install libncursesw5-dev\n"
        "  Fedora/RHEL   : sudo dnf install ncurses-devel\n"
        "설치 후 build 폴더를 비우고 다시 빌드하세요.")
endif()

# 10. 수집된 소스 파일들을 모아서 실행 파일을 만듭니다.
add_executable(${PROJECT_NAME} ${SOURCES})

# 11. 실행 파일과 와이드 ncurses(ncursesw) 라이브러리를 연결(Link)합니다.
target_link_libraries(${PROJECT_NAME} ${NCURSESW_LIB})