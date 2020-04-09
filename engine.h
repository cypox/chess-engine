#pragma once

#include "board.h"

class engine{
public:
  engine(const board& b) : m_board(b) {};
  std::vector<move> ponder();
  static double evaluate(const board&);
  static double value(piece&);
  void update_board(const board&);

private:
  board m_board;
};
