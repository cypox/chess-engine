#pragma once
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>

#include "board.h"

class board_hash {
  public:
  size_t operator()(const board& b) const { // Zobrist hashing from wikipedia
    size_t h = 0;
    for(int i = 0 ; i < 8 ; ++ i)
    {
      for (int j= 0 ; j < 8 ; ++ j)
      {
        if (b.at(i, j) != EMPTY)
        {
          h = h ^ b.at(i, j);
        }
      }
    }
    return h;
  }
};

class engine{
public:
  engine(const board& b) : m_board(b) {};
  double minmax(board&, int);
  std::vector<move> ponder(const board);
  std::vector<move> ponder();
  static double evaluate(const board&);
  static double value(piece&);
  void update_board(const board&);
  std::vector<move> get_possible_moves(const board&);

private:
  board m_board;
  int evaluated = 0;
  std::unordered_map<board, std::vector<move>, board_hash> possible_moves;
};
