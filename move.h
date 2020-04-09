#pragma once

#include "piece.h"

class move{
  public:
  move(piece p, uint8 x, uint8 y) : m_piece(p), destination_x(x), destination_y(y) {};
  move(piece p, uint8 x, uint8 y, p_type t) : m_piece(p), destination_x(x), destination_y(y), promotion(t) {};
  void set_capture(const bool val) {capture = val;};
  void set_check(const bool val) {check = val;};

  std::string to_str(std::vector<move>& moves);

  piece m_piece;
  uint8 destination_x;
  uint8 destination_y;
  p_type promotion = EMPTY;
  private:
  std::string piece_id(p_type);
  std::string case_id(uint8, uint8);
  // TODO add these
  bool check = false;
  bool checkmate = false;
  bool capture = false;
};
