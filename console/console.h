#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>

namespace console {
// FPS
const int FPS = 60;
// 화면에 출력 가능한 최대 너비
const int SCREEN_WIDTH = 25;
// 화면에 출력 가능한 최대 높이
const int SCREEN_HEIGHT = 25;

void init();

void wait();

void draw(int x, int y, const char *s);

void draw(int x, int y, std::string s);

void drawBox(int minX, int minY, int maxX, int maxY);

void log(std::string s);

void clear();

enum Key {
  K_NONE = 0,
  K_OTHER,
  K_LEFT,
  K_RIGHT,
  K_UP,
  K_DOWN,
  K_ESC,
  K_SPACE,
  K_Z,
  K_X,
  K_ENTER
};

// 키가 눌렸는지 확인한다.
bool key(Key k);

}

#endif
