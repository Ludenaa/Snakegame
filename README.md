# 🐍 SnakeGame

> 26.1 KMU C++ 프로그래밍 기말 프로젝트 9조
>
> ncurses 기반 터미널 스네이크 게임

---

## 개발 환경

| 항목 | 내용 |
|------|------|
| OS | Linux (Ubuntu/Debian 기준), macOS |
| 언어 | C++17 |
| 빌드 | CMake 3.10 이상, Make, g++ |
| 의존성 | ncursesw (와이드 문자 출력용 ncurses) |

> 유니코드 블록 문자(■) 출력을 위해 일반 `ncurses`가 아닌 **와이드 버전(`ncursesw`)**이 필요합니다.

---

## 빌드 및 실행

### 의존성 설치

```bash
# Ubuntu / Debian
sudo apt-get install build-essential cmake libncurses5-dev libncursesw5-dev

# Fedora / RHEL
sudo dnf install gcc-c++ cmake ncurses-devel

# macOS
brew install cmake ncurses
```

### 빌드 & 실행 (자동)

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

실행 파일은 `bin/SnakeGame` 에 생성됩니다.

---

## 조작 방법

| 키 | 동작 |
|----|------|
| `↑` `↓` `←` `→` | 이동 방향 전환 |
| `q` | 게임 종료 |

---

## 게임 규칙

### 기본 규칙

- 뱀의 최소 길이는 **3** — 그 이하로 줄어들면 게임 오버
- 일반 벽(Wall)·통과 불가 벽(Immune Wall) 또는 자기 몸에 충돌하면 게임 오버

### 아이템

| 아이템 | 효과 |
|--------|------|
| **Growth** | 길이 +1 |
| **Poison** | 길이 -1 (Shield 보유 시 Shield가 대신 소모되어 길이 유지) |
| **Shield** | 획득 시 다음 Poison 1회를 방어 (보유 중에는 뱀 색상이 변경됨) |

- 아이템은 빈 공간에 무작위로 생성되며, **동시에 최대 3개**까지 존재합니다.
- 생성된 아이템은 일정 시간(약 10초)이 지나면 자동으로 사라집니다.

### 게이트(Gate)

- 일정 시간(약 10초)마다 벽 위 두 곳에 한 쌍의 게이트가 생성됩니다.
- 한쪽 게이트로 진입하면 반대편 게이트로 빠져나오며, 진출 방향은 벽의 위치(가장자리/코너)에 따라 결정됩니다.
- Immune Wall 위에는 게이트가 생성되지 않습니다.

### 난이도 & 스테이지

미션을 달성하면 다음 스테이지로 진행하며, 단계가 올라갈수록 맵이 커지고 속도가 빨라집니다.

| 스테이지 | 맵 크기 | 속도 | 아이템 생성 비율 (Growth : Poison : Shield) |
|----------|---------|------|------|
| Easy | 21 × 21 | 느림 | 1 : 1 : 1 |
| Normal | 25 × 25 | 보통 | 1 : 1 : 1 |
| Hard | 31 × 31 | 빠름 | 1 : 1 : 0 |
| Extreme | 35 × 35 | 매우 빠름 | 1 : 2 : 0 |

### 미션 시스템

각 스테이지에는 점수판(ScoreBoard) 옆에 미션 목표가 표시됩니다. 아래 항목의 목표치를 모두 달성하면 해당 스테이지를 클리어합니다.

- **B** : 현재 길이 / 최대 길이 (목표 길이 달성 여부)
- **+** : 획득한 Growth 개수
- **-** : 획득한 Poison 개수
- **G** : Gate 사용 횟수

모든 스테이지(Extreme까지)를 클리어하면 승리, 도중에 게임 오버되면 패배 화면이 표시됩니다.

---

## 화면 표시

맵의 모든 오브젝트는 **색으로 구분되는 `■` 블록**으로 표시됩니다. (ASCII 기호가 아닌 색상으로 구분)

| 표시 | 의미 |
|------|------|
| 흰색 ■ | 일반 벽 (Wall) |
| 회색 ■ | Immune Wall (게이트 생성 불가) |
| 밝은 초록 ■ | 뱀 머리 |
| 초록 ■ | 뱀 몸통 |
| 하늘색 ■ | Shield 보유 중인 뱀 |
| 빨강 ■ | Growth Item |
| 보라 ■ | Poison Item |
| 파랑 ■ | Shield Item |
| 청록 ■ | Gate |
| (빈 칸) | 빈 공간 |

> 256색을 지원하는 터미널에서는 세련된 색상을, 그렇지 않으면 기본 8색으로 자동 대체합니다.

---

## 프로젝트 구조

```
Snakegame/
├── CMakeLists.txt          # CMake 빌드 구성 (C++17, ncursesw 링크)
├── README.md               # 프로젝트 설명 / 빌드·실행 / 규칙
├── build.sh                # 빌드 자동화 스크립트 (build·bin 생성 → cmake → make → 실행)
├── structure.txt           # 프로젝트 구조 및 네이밍 규칙
│
├── data/                   # 맵 데이터 (난이도별)
│   ├── map1.txt            #  - Easy    (21x21)
│   ├── map2.txt            #  - Normal  (25x25)
│   ├── map3.txt            #  - Hard    (31x31)
│   └── map4.txt            #  - Extreme (35x35)
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
```

### 클래스 구성

| 클래스 | 역할 |
|--------|------|
| `Map` | 게임판(board) |
| `Snake` | 뱀 |
| `Item` | 아이템 (Growth / Poison / Shield 통합) |
| `Gate` | 출입구 |
| `ScoreBoard` | 점수판·미션 (요구 목록 외 추가 클래스) |
| `Config` | 설정 (클래스 아님 — 구조체 + 팩토리 함수 `makeConfig`) |

---

## 팀원 및 역할

| 이름 | 담당 |
|------|------|
| 이명준 (팀장) | Snake |
| 이지형 | Map |
| 정재운 | Item |
| 정원우 | ScoreBoard, Gate, 빌드 구성(CMake/build), 프로젝트 구조 |

---

## 코딩 컨벤션

| 대상 | 표기법 |
|------|--------|
| 클래스 / 구조체 / 메소드 | PascalCase |
| 멤버 함수 | camelCase |
| 함수 / 변수 / 멤버 변수 | snake_case |