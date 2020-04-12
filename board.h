#pragma once
#include <vector>
#include <bits/stdc++.h>
#include <iostream>

#include "piece.h"
#include "move.h"

class move;

class board {
  public:
  board();
  board(std::vector<std::vector<p_type> >& new_state, bool to_play) : m_state(new_state), white_to_play(to_play) {};

  bool operator==(const board&) const;

  void switch_player() {white_to_play = !white_to_play;};
  void print_board();

  std::vector<piece> get_player_pieces(bool) const;
  std::vector<move> get_possible_moves(bool) const;
  std::vector<move> get_moves_after_simulation(bool, move&) const;
  std::vector<move> get_piece_moves(piece&, const std::vector<std::vector<p_type> >&) const;
  bool process_move(move&) const;
  bool is_collision(move&) const;
  bool will_be_in_check(bool, std::vector<std::vector<p_type> >&) const;
  void do_move(move&);
  void undo_move(move&);
  board* simulate_move(move&) const;

  void play_unsafe(move&);
  bool to_play() const {return white_to_play;};

  inline p_type at(int i, int j) const {return m_state[i][j];};

  private:
  std::vector<std::vector<p_type> > m_state;
  bool white_to_play = true;
};
