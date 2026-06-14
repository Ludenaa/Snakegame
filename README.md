# 🐍 SnakeGame

> 26.1 KMU  C++ 프로그래밍 기말 프로젝트 9조
> 
> ncurses 기반 터미널 스네이크 게임

---

## 개발 환경

| 항목 | 내용 |
|------|------|
| OS   | Linux|
| 언어 | C++17 |
| 빌드 | CMake 3.10 이상 |
| 의존성 | ncurses |

---

## 빌드 및 실행

### 의존성 설치

```bash
# Ubuntu / Debian
sudo apt-get install libncurses5-dev libncursesw5-dev

# macOS
brew install ncurses
```

### 빌드 & 실행

```bash
git clone https://github.com/Ludenaa/Snakegame.git
cd Snakegame
chmod +x build.sh
./build.sh
```

빌드 성공 시 자동으로 게임이 실행됩니다.

### 수동 빌드

```bash
mkdir -p build bin
cd build
cmake ..
make
../bin/SnakeGame
```

---

## 조작 방법

| 키 | 동작 |
|----|------|
| `↑` `↓` `←` `→` | 방향 전환 |
| `q` | 게임 종료 |

---

## 게임 규칙

- 뱀의 최소 길이는 **3** — 그 이하가 되면 게임 오버
- 벽 또는 자기 몸에 충돌하면 게임 오버
- **Growth Item (+)** 을 먹으면 길이 +1
- **Poison Item (-)** 을 먹으면 길이 -1
- **Gate (G)** 를 통과하면 반대편 게이트로 이동

---

## 타일 표시

| 기호 | 의미 |
|------|------|
| `#` | 일반 벽 |
| `X` | Immune Wall (게이트 생성 불가) |
| `H` | 뱀 머리 |
| `B` | 뱀 몸통 |
| `G` | Growth Item |
| `P` | Poison Item |
| `.` | 빈 공간 |

---

## 프로젝트 구조

```
Snakegame/
├── CMakeLists.txt          # CMake 빌드 구성 (C++17, ncursesw 링크)
├── README.md               # 프로젝트 설명 / 빌드·실행 / 팀원 역할
├── build.sh                # 빌드 자동화 스크립트 (build·bin 생성 → cmake → make → 실행)
├── structure.txt           # 본 파일: 프로젝트 구조 및 네이밍 규칙
│
├── data/                   # 맵 데이터 (난이도별)
│   ├── map1.txt            #  - Easy   (21x21)
│   ├── map2.txt            #  - Normal (25x25)
│   ├── map3.txt            #  - Hard   (31x31)
│   └── map4.txt            #  - Extreme(35x35)
│
├── include/                # 헤더 파일 (.hpp)
│   ├── Config.hpp          #  - 난이도/맵규격/미션목표 설정 구조체 + makeConfig()
│   ├── Map.hpp             #  - 게임판(board) 데이터·렌더링 인터페이스
│   ├── Snake.hpp           #  - 뱀 이동/성장/충돌 처리 인터페이스
│   ├── Item.hpp            #  - 아이템(Growth/Poison/Shield) 생성·만료 관리
│   ├── Gate.hpp            #  - 게이트 쌍 생성/제거·진출 좌표 계산
│   └── ScoreBoard.hpp      #  - 점수판·미션 상태 관리
│
├── src/                    # 소스 파일 (.cpp)
│   ├── main.cpp            #  - 진입점: ncurses 초기화 + 스테이지 진행 메인 루프
│   ├── Config.cpp          #  - makeConfig(): 난이도 → 파라미터 매핑
│   ├── Map.cpp             #  - 맵 로드 + 더블버퍼링 렌더링
│   ├── Snake.cpp           #  - 뱀 로직 구현
│   ├── Item.cpp            #  - 아이템 로직 구현
│   ├── Gate.cpp            #  - 게이트 로직 구현
│   └── ScoreBoard.cpp      #  - 점수판 로직 구현
│
├── bin/                    # (자동 생성) 컴파일된 실행 파일 SnakeGame
└── build/                  # (자동 생성) CMake 빌드 중간 파일
 
─────────────────────────────────────────────
네이밍 규칙 (Naming Convention)
─────────────────────────────────────────────
  클래스 / 구조체 / 메소드 : PascalCase
  멤버 함수                : camelCase
  함수 / 변수 / 멤버 변수  : snake_case
 
─────────────────────────────────────────────
클래스 구성 (6개)
─────────────────────────────────────────────
  Map        : 게임판(board)
  Snake      : 뱀
  Item       : 아이템 (Growth / Poison / Shield 통합)
  Gate       : 출입구
  ScoreBoard : 점수판 (요구 목록 외 추가 클래스)
  + Config   : 설정 (클래스 아님, 구조체 + 팩토리 함수)
```

---

## 팀원 및 역할

| 이름 | 담당 |
|------|------|
| 이명준(팀장) | Snake  |
| 이지형 | Map  |
| 정재운 | Item  |
| 정원우 | ScoreBoard, Gate  |

---

## 코딩 컨벤션

| 대상 | 표기법 |
|------|--------|
| 클래스, 구조체 | PascalCase |
| 멤버 함수 | camelCase |
| 멤버 변수 | snake_case |
