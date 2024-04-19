#include "tetromino.h"
#include "console/console.h"
#include <iostream>

Tetromino::Tetromino(std::string name, int size, std::string shape) {
  name_ = name;
  size_ = size;

  for (int i = 0; i < MAX_SIZE; i++) { // Init
    for (int j = 0; j < MAX_SIZE; j++) {
      shape_[i][j] = false;
    }
  }
  
  for (int i = 0; i < size; i++) { // Generate
    for (int j = 0; j < size; j++) {
      shape_[j][i] = shape[i * size + j] == 'O';
    }
  }

  original_ = new Tetromino(*this);
}

Tetromino Tetromino::rotatedCW() {
  Tetromino rotated = *this;
  for (int i = 0; i < size_; i++) {
    for (int j = 0; j < size_; j++) {
      rotated.shape_[j][size_ - i - 1] = shape_[i][j];
    }
  }
  // console::log("rotatedCW()");
  return rotated;
}

Tetromino Tetromino::rotatedCCW() {
  Tetromino rotated = *this;
  for (int i = 0; i < size_; i++) {
    for (int j = 0; j < size_; j++) {
      rotated.shape_[size_ - j - 1][i] = shape_[i][j];
    }
  }
  // console::log("rotatedCCW()");
  return rotated;
}
  
void Tetromino::drawAt(std::string s, int x, int y) {
  for (int i = 0; i < size_; i++) {
    for (int j = 0; j < size_; j++) {
      if (shape_[i][j]) {
        console::draw(x + i, y + j, s);
      }
    }
  }
}

// 각 테트로미노 종류에 대한 구현
Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");