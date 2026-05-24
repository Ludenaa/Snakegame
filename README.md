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
├── bin/            # 실행 파일
├── build/          # CMake 빌드 중간 파일
├── data/
│   └── map1.txt    # 맵 데이터
├── include/        # 헤더 파일
│   ├── Item.hpp
│   ├── Map.hpp
│   ├── ScoreBoard.hpp
│   └── Snake.hpp
├── src/            # 소스 파일
│   ├── main.cpp
│   ├── Item.cpp
│   ├── Map.cpp
│   ├── ScoreBoard.cpp
│   └── Snake.cpp
├── build.sh        # 빌드 스크립트
└── CMakeLists.txt
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
