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

struct {
  bool operator()(move& a, move& b) const
  {
    double lhs = a.get_score();
    double rhs = b.get_score();
    return lhs > rhs;
  }
} sort_by_move_scores;

double engine::minmax(board& state, int depth)
{
  if (depth == 0)
  {
    evaluated ++;
    return engine::evaluate(state);
  }
  else
  {
    std::vector<move> moves = get_possible_moves(state);
    double max = -100.0;
    double min = 100.0;
    for (auto m : moves)
    {
      board* temp_board = state.simulate_move(m);
      double branch_value = minmax(*temp_board, depth-1);
      m.set_score(branch_value);
      delete temp_board;
      if (branch_value > max)
        max = branch_value;
      if (branch_value < min)
        min = branch_value;
    }
    return state.to_play()?max:min;
  }
}

std::vector<move> engine::ponder(const board state)
{
  std::vector<move> moves = get_possible_moves(state);
  int len = moves.size();
  for (int i = 0 ; i < len ; ++ i)
  {
      board* temp_board = state.simulate_move(moves[i]);
      double branch_value = minmax(*temp_board, 2);
      moves[i].set_score(branch_value);
      delete temp_board;
  }
  std::sort(moves.begin(), moves.end(), sort_by_move_scores);
  std::cout << "evaluated : " << evaluated << std::endl;
  evaluated = 0;
  return moves;
}

std::vector<move> engine::ponder()
{
  std::vector<move> moves = get_possible_moves(m_board);
  sort_by_piece_value.local_board = &m_board;
  std::sort(moves.begin(), moves.end(), sort_by_piece_value);
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

std::vector<move> engine::get_possible_moves(const board& b)
{
  if (possible_moves.find(b) == possible_moves.end())
  {
    possible_moves[b] = b.get_possible_moves(b.to_play());
  }
  return possible_moves[b];
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
