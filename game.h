
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 5

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  int frame = 0;
  int lines_left = LINES;
  int gameState = 0; // 0; NONE, 1; WIN, 2; LOST
  std::string time_string;
  Tetromino *tetrominos[7] = {&Tetromino::I, &Tetromino::O, &Tetromino::T, &Tetromino::S, &Tetromino::Z, &Tetromino::J, &Tetromino::L};
  Tetromino *curTetro;
  Tetromino *nextTetro;
  Tetromino *holdTetro;
  Tetromino *shadowTetro;
  int curX, curY;
  int shaX, shaY;
  bool canHold = true;

  bool isTetroAtBottom();
  bool isCollision();
  bool isHitWall();
  bool canMoveTo(Tetromino *tetro, int dx, int dy);
  bool canCastTo(Tetromino *tetro, int dx, int dy);
  bool canRotate(Tetromino *tetro);
  void clearLine();
  bool canSpawn(Tetromino *tetro);
  void toBoard();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif