#include "game.h"
#include "console/console.h"
#include <string.h>
#include <ctime>

void Game::update() {
  frame++; // Frame update

  int total = frame * 1000 / 60; // Timer update
  int min = total / 60000;
  int sec = (total / 1000) % 60;
  int mil = (total / 10) % 100;
  time_string =
      (min < 10 ? "0" : "") + std::to_string(min) + ":" +
      (sec < 10 ? "0" : "") + std::to_string(sec) + ":" +
      (mil < 10 ? "0" : "") + std::to_string(mil);

  shadowTetro = curTetro;
  shaX = curX;
  shaY = curY;

  if (console::key(console::K_LEFT) && canMoveTo(curTetro, -1, 0)) { curX--; }
  if (console::key(console::K_RIGHT) && canMoveTo(curTetro, 1, 0)) { curX++; }
  if (console::key(console::K_DOWN)) {
    if (canMoveTo(curTetro, 0, 1)) {
      curY++;
    } else { // for the fast stacking!
      toBoard();
    }
  }

  if (console::key(console::K_UP)) { // hard drop
    while (true) {
      if (canMoveTo(curTetro, 0, 1)) {
        curY++;
      } else {
        toBoard();
        break;
      }
    }
  }

  while (true) {
    if (canCastTo(shadowTetro, 0, 1)) {
      shaY++;
    } else {
      break;
    }
  }

  if (console::key(console::K_X)) { // rotate CW
    Tetromino *rotated = new Tetromino(*curTetro); // copy
    *rotated = curTetro->rotatedCW();
    if (canRotate(rotated)) {
      *curTetro = curTetro->rotatedCW();
    } else {
      delete rotated;
    }
  }

  if (console::key(console::K_Z)) { // rotate CCW
    Tetromino *rotated = new Tetromino(*curTetro); // copy
    *rotated = curTetro->rotatedCCW();
    if (canRotate(rotated)) {
      *curTetro = curTetro->rotatedCCW();
    } else {
      delete rotated;
    }
  }

  if (console::key(console::K_SPACE) && canHold) { // hold
    if (holdTetro == nullptr) {
      holdTetro = new Tetromino(*curTetro->original());
      delete curTetro;
      curTetro = new Tetromino(*nextTetro);
      delete nextTetro;
      nextTetro = new Tetromino(*tetrominos[rand() % 7]);
      canHold = false;
    } else {
      Tetromino *temp = holdTetro;
      holdTetro = new Tetromino(*curTetro->original());
      delete curTetro;
      curTetro = temp;
      canHold = false;
    }
    curX = 6 - curTetro->size() / 2;
    curY = 1;
  }

  if (frame % DROP_DELAY == 0) { // auto falling & check collision
    if (!isTetroAtBottom() && !isCollision()) {
      curY++;
    } else { // Does collide?
      toBoard();
    }
  }

  clearLine();

  if (lines_left <= 0) { // Game state update
    gameState = 1;
  }
}

void Game::draw() {
  console::drawBox(0, 0, 11, 21); // Tetris box field
  console::drawBox(12, 0, 17, 5); // Next field
  if (nextTetro->size() < 4) {
    nextTetro->drawAt(BLOCK_STRING, 15 - nextTetro->size() / 2, 2);
  } else {
    nextTetro->drawAt(BLOCK_STRING, 15 - nextTetro->size() / 2, 1);
  }
  console::draw(13, 0, "Next");
  console::drawBox(18, 0, 23, 5); // Hold field
  if (holdTetro != nullptr && holdTetro->size() < 4) {
    holdTetro->drawAt(BLOCK_STRING, 21 - holdTetro->size() / 2, 2);
  } else if (holdTetro != nullptr) {
    holdTetro->drawAt(BLOCK_STRING, 21 - holdTetro->size() / 2, 1);
  }

  console::draw(19, 0, "Hold");
  console::draw(0, 22, std::to_string(lines_left) + " lines left");
  console::draw(2, 23, time_string);

  // Drawing current tetromino
  shadowTetro->drawAt(SHADOW_STRING, shaX, shaY);
  curTetro->drawAt(BLOCK_STRING, curX, curY);

  // Drawing board_
  for (int i = 0; i < BOARD_WIDTH; i++) {
    for (int j = 0; j < BOARD_HEIGHT; j++) {
      if (board_[i][j]) {
        console::draw(i + 1, j + 1, BLOCK_STRING);
      }
    }
  }

  if (gameState == 1) {
    console::draw(3, 6, "You Win");
    console::draw(2, 7, time_string);
  } else if (gameState == 2) {
    console::draw(3, 6, "You Lost");
  }
}

void Game::toBoard() {
  for (int i = 0; i < curTetro->size(); i++) {
    for (int j = 0; j < curTetro->size(); j++) {
      if (curTetro->check(i, j)) {
        board_[curX + i - 1][curY + j - 1] = true;
      }
    }
  }
  delete curTetro;
  if (!canSpawn(nextTetro)) {
    gameState = 2;
    // console::log("gamestate has benen changed to 2.");
  }
  curTetro = nextTetro;
  nextTetro = new Tetromino(*tetrominos[rand() % 7]);
  curX = 6 - curTetro->size() / 2;
  curY = 1;
  canHold = true;
}

bool Game::canSpawn(Tetromino *tetro) {
  int spawnX = 6 - tetro->size() / 2;
  int spawnY = 1;

  for (int i = 0; i < tetro->size(); i++) {
    for (int j = 0; j < tetro->size(); j++) {
      if (tetro->check(i, j)) {
        int x = spawnX + i;
        int y = spawnY + j;
        if (board_[x][y]) { return false; }
      }
    }
  }
  return true;
}

bool Game::canMoveTo(Tetromino *tetro, int dx, int dy) {
  for (int i = 0; i < tetro->size(); i++) {
    for (int j = 0; j < tetro->size(); j++) {
      if (tetro->check(i, j)) {
        int x = curX + i + dx;
        int y = curY + j + dy;
        if (x <= 0 || x >= 11 || y >= 21 || board_[x - 1][y - 1]) { return false; }
      }
    }
  }
  return true;
}

bool Game::canCastTo(Tetromino *tetro, int dx, int dy) {
  for (int i = 0; i < tetro->size(); i++) {
    for (int j = 0; j < tetro->size(); j++) {
      if (tetro->check(i, j)) {
        int x = shaX + i + dx;
        int y = shaY + j + dy;
        if (x <= 0 || x >= 11 || y >= 21 || board_[x - 1][y - 1]) { return false; }
      }
    }
  }
  return true;
}

bool Game::canRotate(Tetromino *tetro) {
  for (int i = 0; i < tetro->size(); i++) {
    for (int j = 0; j < tetro->size(); j++) {
      if (tetro->check(i, j)) {
        int x = curX + i - 1; // -1; to start X from 0
        int y = curY + j - 1; // cuz we're gonna check board_[x][y], which starts from (0, 0)
        if (x < 0 || x > 9 || y > 20 || board_[x][y]) { return false; }
      }
    }
  }
  return true;
}

bool Game::isTetroAtBottom() {
  for (int i = 0; i < curTetro->size(); i++) {
    for (int j = 0; j < curTetro->size(); j++) {
      if (curTetro->check(i, j) && curY + j == BOARD_HEIGHT) {
        return true;
      }
    }
  }
  return false;
}

bool Game::isCollision() {
  for (int i = 0; i < curTetro->size(); i++) {
    for (int j = 0; j < curTetro->size(); j++) {
      if (curTetro->check(i, j) && board_[curX + i - 1][curY + j]) {
        return true;
      }
    }
  }
  return false;
}

bool Game::isHitWall() {
  for (int i = 0; i < curTetro->size(); i++) {
    for (int j = 0; j < curTetro->size(); j++) {
      if (curTetro->check(i, j) && (curX + i <= 0 || curX + i > 10)) {
        return true;
      }
    }
  }
  return false;
}

void Game::clearLine() {
  for (int y = 0; y < BOARD_HEIGHT; y++) {
    bool isFull = true;
    for (int x = 0; x < BOARD_WIDTH; x++) {
      if (!board_[x][y]) { // all false?
        isFull = false;
        break;
      }
    }
    if (isFull) { // all true?
      lines_left--;
      for (int y2 = y; y2 > 0; y2--) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
          board_[x][y2] = board_[x][y2 - 1]; // shifting
        }
      }
    }
  }
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit() {
  if (gameState == 1 || gameState == 2) {
    return true;
  } else {
    return false;
  }
}

Game::Game() {
  for (int i = 0; i < BOARD_WIDTH; i++) {
    for (int j = 0; j < BOARD_HEIGHT; j++) {
      board_[i][j] = false;
    }
  }

  frame = 0;
  lines_left = LINES;
  gameState = 0;
  time_string = "";
  curTetro = new Tetromino(*tetrominos[rand() % 7]);
  nextTetro = new Tetromino(*tetrominos[rand() % 7]);
  holdTetro = nullptr;
  shadowTetro = curTetro;
  curX = 6 - curTetro->size() / 2;
  curY = 1;
  bool canHold = false;
  shaX = curX;
  shaY = curY;
}