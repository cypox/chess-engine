#include <vector>
#include <algorithm>

#include "engine.h"

struct {
  bool operator()(move& a, move& b) const
  {
    board* after_a = local_board->simulate_move(a);
    board* after_b = local_board->simulate_move(b);
    double lhs = engine::evaluate(*after_a);
    double rhs = engine::evaluate(*after_b);
    a.set_score(lhs);
    b.set_score(rhs);
    delete after_a;
    delete after_b;
    return lhs > rhs;
  }
  board* local_board;
} sort_by_piece_value;

/*
std::vector<move> engine:minmax(int depth)
{
  std::vector<move> moves = m_board.get_possible_moves(m_board.to_play());
  if (depth == 0)
  {
    sort_by_piece_value.local_board = &m_board;
    std::sort(moves.begin(), moves.end(), sort_by_piece_value);
    return moves; // this should be the minimum
  }
  else
  {
    for (auto m : moves)
    {
      board* temp_board = m_board.simulate_move(m);
      double branch_value = minmax(depth-1);
      m.set_score(branch_value);
      delete temp_board;
    }
  }
}
*/

std::vector<move> engine::ponder()
{
  std::vector<move> moves = m_board.get_possible_moves(m_board.to_play());
  sort_by_piece_value.local_board = &m_board;
  std::sort(moves.begin(), moves.end(), sort_by_piece_value);
  /*
  for (auto s : moves)
  {
    std::cout << s.to_str(moves) << " (" << s.get_score() << ")     " << std::endl;
  }
  */
  return moves;
}

double engine::evaluate(const board& m_board)
{
  bool player = m_board.to_play();
  // pieces
  double white_sum = 0.0, black_sum = 0.0;
  std::vector<piece> white_pieces = m_board.get_player_pieces(true);
  for (auto p : white_pieces)
  {
    white_sum += engine::value(p);
  }
  std::vector<piece> black_pieces = m_board.get_player_pieces(false);
  for (auto p : black_pieces)
  {
    black_sum += engine::value(p);
  }
  return white_sum + black_sum;
}

double engine::value(piece& p)
{
  double value = 0.0f;
  switch(p.type)
  {
    case W_PAWN:
      value = 1.0f;
      break;
    case B_PAWN:
      value = -1.0f;
      break;
    case W_KNIGHT:
      value = 3.0f;
      break;
    case B_KNIGHT:
      value = -3.0f;
      break;
    case W_BISHOP:
      value = 3.2f;
      break;
    case B_BISHOP:
      value = -3.2f;
      break;
    case W_ROOK:
      value = 5.0f;
      break;
    case B_ROOK:
      value = -5.0f;
      break;
    case W_QUEEN:
      value = 9.0f;
      break;
    case B_QUEEN:
      value = -9.0f;
      break;
    case W_KING:
    case B_KING:
    default:
      value = 0.0f;
      break;
  }
  return value;
}

void engine::update_board(const board& b)
{
  m_board = b;
}
