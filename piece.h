#pragma once
#include <string>
#include <vector>

typedef int uint8;

enum p_type {
  EMPTY = 0,
  W_PAWN = 1,
  W_KNIGHT = 2,
  W_BISHOP = 3,
  W_ROOK = 4,
  W_QUEEN = 5,
  W_KING = 6,
  B_PAWN = 7,
  B_KNIGHT = 8,
  B_BISHOP = 9,
  B_ROOK = 10,
  B_QUEEN = 11,
  B_KING = 12
};

class piece {
  public:
  piece() : x(0), y(0), type(W_PAWN) {};
  piece(uint8 x, uint8 y, p_type s) : x(x), y(y), type(s) {};
  piece& operator=(const piece&);

  uint8 x;
  uint8 y;
  p_type type;
  private:
};
