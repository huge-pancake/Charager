#include "patchesforlinux.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>

// 1 millisecond = 1000 microsecond
#define MS 1000
#define S 1000000

#define WIDTH 40
#define HEIGHT 15
#define MV_LEFT 0
#define MV_DOWN 1
#define MV_UP 2
#define MV_RIGHT 3

#define BLOCK_VOID 0
#define BLOCK_STONE 1
#define BLOCK_STONE_SMALL 2
#define BLOCK_TREE 3
#define BLOCK_TREE_SMALL 4
#define BLOCK_IRON 5
#define BLOCK_GOLD 6

class Player {
private:
public:
  bool inGame = false;
  unsigned short x = 0;
  unsigned short y = 0;
  unsigned short lastX = 0;
  unsigned short lastY = 0;
  unsigned short stepSize = 1;
  void move(int direction) {
    lastX = x;
    lastY = y;
    switch (direction) {
    case MV_LEFT:
      x - stepSize < 0 ? x = 0 : x -= stepSize;
      break;
    case MV_DOWN:
      y + stepSize > HEIGHT - 1 ? y = HEIGHT - 1 : y += stepSize;
      break;
    case MV_UP:
      y - stepSize < 0 ? y = 0 : y -= stepSize;
      break;
    case MV_RIGHT:
      x + stepSize > WIDTH - 1 ? x = WIDTH - 1 : x += stepSize;
      break;
    default:
      break;
    }
  }
};

Player player;

//          foreground background
// black        30         40
// red          31         41
// green        32         42
// yellow       33         43
// blue         34         44
// magenta      35         45
// cyan         36         46
// white        37         47
// \033[1;36m{{TEXTS}}\033[0m

void clearScreen() { printf("\033[2J\033[1;1H"); }
int randomNumberIn(int min, int max) { return rand() % (max - min + 1) + min; }

unsigned short _map[HEIGHT][WIDTH];

void update() {
  int generateLv1 = randomNumberIn(0, 60 * 30);
  if (generateLv1 == 0) {
    unsigned short x = randomNumberIn(0, WIDTH);
    unsigned short y = randomNumberIn(0, HEIGHT);
    unsigned short what = randomNumberIn(1, 4);
    _map[x][y] = _map[x][y] ? _map[x][y] : what;
  }
  int generateLv2 = randomNumberIn(0, 60 * 40);
  if (generateLv2 == 0) {
    unsigned short x = randomNumberIn(0, WIDTH);
    unsigned short y = randomNumberIn(0, HEIGHT);
    unsigned short what = randomNumberIn(5, 6);
    _map[x][y] = _map[x][y] ? _map[x][y] : what;
  }
}

void resetMap() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      _map[j][i] = 0;
    }
  }
  for (int i = 0; i < 10800; i++) {
    update();
  }
}

char transformPoint(unsigned short pointCode) {
  switch (pointCode) {
  case BLOCK_VOID:
    return '.';
  case BLOCK_STONE:
    return 'S';
  case BLOCK_STONE_SMALL:
    return 's';
  case BLOCK_TREE:
    return 'T';
  case BLOCK_TREE_SMALL:
    return 't';
  case BLOCK_IRON:
    return 'Q';
  case BLOCK_GOLD:
    return 'Q';
  default:
    return '?';
  }
}

void renderMap() {
  gotoxy(0, 0);
  unsigned short _temp;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (j == player.x && i == player.y) {
        printf("\033[1;31m@\033[0m");
        continue;
      }
      _temp = _map[j][i];
      switch (_temp) {
      case BLOCK_VOID:
        printf("\033[1;0m%c\033[0m", transformPoint(_temp));
        break;
      case BLOCK_STONE:
      case BLOCK_STONE_SMALL:
      case BLOCK_GOLD:
        printf("\033[1;33m%c\033[0m", transformPoint(_temp));
        break;
      case BLOCK_IRON:
        printf("\033[1;37m%c\033[0m", transformPoint(_temp));
        break;
      case BLOCK_TREE:
      case BLOCK_TREE_SMALL:
        printf("\033[1;32m%c\033[0m", transformPoint(_temp));
      }
    }
    printf("          \n");
  }
}
void render() {
  // clearScreen();
  gotoxy(0, HEIGHT);
  renderMap();
  printf("{ x: %3d, y: %3d }          \n", player.x, player.y);
}

bool flag = true;
bool gameLoop() {
  if (kbhit()) {
    char _ = getch();
    switch (_) {
    // movement
    case 'h':
    case 'a':
      player.move(MV_LEFT);
      break;
    case 'j':
    case 's':
      player.move(MV_DOWN);
      break;
    case 'k':
    case 'w':
      player.move(MV_UP);
      break;
    case 'l':
    case 'd':
      player.move(MV_RIGHT);
      break;
    case 'r':
      resetMap();
      break;
    case 'q':
      flag = false;
      break;
    default:
      break;
    }
  }
  update();
  render();
  usleep(16 * MS);
  return flag;
}

int main() {
  clearScreen();
  srand(time(NULL));
  resetMap();
  while (gameLoop()) {
  }
  return 0;
}
