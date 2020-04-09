#pragma once
#include <vector>
#include <iostream>

#include "piece.h"
#include "move.h"

class move;

class board {
  public:
  board();

  void switch_player() {white_to_play = !white_to_play;};
  void print_board();

  std::vector<piece> get_player_pieces(bool player) const;
  std::vector<move> get_possible_moves(bool player) const;
  std::vector<move> get_all_moves_with_checks(bool player) const;
  std::vector<move> get_moves_after_simulation(bool player, move&) const;
  std::vector<move> get_piece_moves(piece&) const;
  bool process_move(move&) const;
  bool is_collision(move&) const;
  bool will_be_in_check(move&) const;
  bool will_check(move&) const;

  void play_unsafe(move&);
  bool to_play() const {return white_to_play;};

  private:
  std::vector<std::vector<p_type> > m_state;
  bool white_to_play = true;
};
