#include "board.h"

bool board::will_be_in_check(bool is_white, std::vector<std::vector<p_type> >& new_state) const {
  int ENNEMY_PAWN = (is_white) * 6 + W_PAWN;
  int ENNEMY_KNIGHT = (is_white) * 6 + W_KNIGHT;
  int ENNEMY_BISHOP = (is_white) * 6 + W_BISHOP;
  int ENNEMY_ROOK = (is_white) * 6 + W_ROOK;
  int ENNEMY_QUEEN = (is_white) * 6 + W_QUEEN;
  int ENNEMY_KING = (is_white) * 6 + W_KING;
  piece king;
  std::vector<piece> ennemy_pieces;
  for (int i = 0 ; i < 8 ; ++ i)
  {
    for (int j = 0 ; j < 8 ; ++ j)
    {
      p_type p = new_state[i][j];
      if ((p == W_KING && (is_white == true)) || (p == B_KING && (is_white == false)) )
      {
        king = piece(i, j, p);
      }
      if (p == ENNEMY_PAWN || p == ENNEMY_KNIGHT || p == ENNEMY_BISHOP || p == ENNEMY_ROOK || p == ENNEMY_QUEEN || p == ENNEMY_KING)
      {
        ennemy_pieces.emplace_back(i, j, p);
      }
    }
  }
  for (auto p : ennemy_pieces)
  {
    std::vector<move> ennemy_moves = get_piece_moves(p, new_state);
    for (auto m : ennemy_moves)
    {
      if (m.destination_x == king.x && m.destination_y == king.y)
        return true;
    }
  }
  return false;
}

bool board::is_collision(move& m) const {
  bool collide = false;
  if(m_state[m.destination_x][m.destination_y] != EMPTY)
  {
    uint8 range_min = m.m_piece.type<7?1:7;
    uint8 range_max = m.m_piece.type<7?6:12;
    collide = (m_state[m.destination_x][m.destination_y]>=range_min && m_state[m.destination_x][m.destination_y]<=range_max);
    m.set_capture(!collide);
  }
  return collide;
}

std::vector<piece> board::get_player_pieces(bool is_white) const {
  std::vector<piece> pieces;
  uint8 range_min = is_white?1:7;
  uint8 range_max = is_white?6:12;
  for (int i = 0 ; i < 8 ; ++ i )
  {
    for (int j = 0 ; j < 8 ; ++ j )
    {
      if (m_state[i][j] >= range_min && m_state[i][j] <= range_max)
        pieces.emplace_back(i, j, m_state[i][j]);
    }
  }
  return pieces;
}

bool board::process_move(move& m) const {
  bool possible_move = !is_collision(m);
  if (possible_move)
  {
    // TODO maybe modify m_state rather than copy
    std::vector<std::vector<p_type> > new_state = m_state;
    new_state[m.destination_x][m.destination_y] = m.m_piece.type;
    new_state[m.m_piece.x][m.m_piece.y] = EMPTY;
    possible_move = possible_move && !will_be_in_check(white_to_play, new_state);
    m.set_check(will_be_in_check(!white_to_play, new_state));
  }
  return possible_move;
}

std::vector<move> board::get_possible_moves(bool is_white) const {
  std::vector<move> moves;
  std::vector<piece> pieces = get_player_pieces(is_white);
  for (auto p:pieces) {
    std::vector<move> possible = get_piece_moves(p, m_state);
    for (auto m:possible) {
      if (process_move(m))
        moves.push_back(m);
    }
  }
  return moves;
}

void board::do_move(move& m) {
  // TODO use this with undo_move to save space
}

void board::undo_move(move&m) {
  // TODO use this with do_move to save space
}

board* board::simulate_move(move& m) const {
  std::vector<std::vector<p_type> > new_state = m_state;
  new_state[m.destination_x][m.destination_y] = m.m_piece.type;
  new_state[m.m_piece.x][m.m_piece.y] = EMPTY;
  return new board(new_state, !white_to_play);
}

void board::play_unsafe(move& m) {
  if (m.promotion == EMPTY)
  {
    m_state[m.destination_x][m.destination_y] = m.m_piece.type;
    m_state[m.m_piece.x][m.m_piece.y] = EMPTY;
  }
  else
  {
    m_state[m.destination_x][m.destination_y] = m.promotion;
    m_state[m.m_piece.x][m.m_piece.y] = EMPTY;
  }
  switch_player();
}

std::vector<move> board::get_piece_moves(piece& p, const std::vector<std::vector<p_type> >& m_state) const {
  std::vector<move> moves;
  switch (p.type) {
    case W_PAWN:
      if (p.x+1 < 7 && m_state[p.x+1][p.y] == EMPTY)
        moves.emplace_back(p, p.x+1, p.y);
      if (p.x < 2 && m_state[p.x+2][p.y] == EMPTY && m_state[p.x+1][p.y] == EMPTY)
        moves.emplace_back(p, p.x+2, p.y);
      if (p.x+1 < 7 && p.y+1 < 8 && m_state[p.x+1][p.y+1] != EMPTY)
        moves.emplace_back(p, p.x+1, p.y+1);
      if (p.x+1 < 7 && p.y >= 1 && m_state[p.x+1][p.y-1] != EMPTY)
        moves.emplace_back(p, p.x+1, p.y-1);
      if (p.x+1 == 7 && m_state[p.x+1][p.y] == EMPTY)
      {
        moves.emplace_back(p, p.x+1, p.y, W_QUEEN);
        moves.emplace_back(p, p.x+1, p.y, W_ROOK);
        moves.emplace_back(p, p.x+1, p.y, W_BISHOP);
        moves.emplace_back(p, p.x+1, p.y, W_KNIGHT);
      }
      if (p.x+1 == 7 && p.y+1 < 8 && m_state[p.x+1][p.y+1] != EMPTY)
      {
        moves.emplace_back(p, p.x+1, p.y+1, W_QUEEN);
        moves.emplace_back(p, p.x+1, p.y+1, W_ROOK);
        moves.emplace_back(p, p.x+1, p.y+1, W_BISHOP);
        moves.emplace_back(p, p.x+1, p.y+1, W_KNIGHT);
      }
      if (p.x+1 == 7 && p.y >= 1 && m_state[p.x+1][p.y-1] != EMPTY)
      {
        moves.emplace_back(p, p.x+1, p.y-1, W_QUEEN);
        moves.emplace_back(p, p.x+1, p.y-1, W_ROOK);
        moves.emplace_back(p, p.x+1, p.y-1, W_BISHOP);
        moves.emplace_back(p, p.x+1, p.y-1, W_KNIGHT);
      }
      break;
    case B_PAWN:
      if (p.x > 1 && m_state[p.x-1][p.y] == EMPTY)
        moves.emplace_back(p, p.x-1, p.y);
      if (p.x >= 6 && m_state[p.x-2][p.y] == EMPTY && m_state[p.x-1][p.y] == EMPTY)
        moves.emplace_back(p, p.x-2, p.y);
      if (p.x > 1 && p.y+1 < 8 && m_state[p.x-1][p.y+1] != EMPTY)
        moves.emplace_back(p, p.x-1, p.y+1);
      if (p.x > 1 && p.y >= 1 && m_state[p.x-1][p.y-1] != EMPTY)
        moves.emplace_back(p, p.x-1, p.y-1);
      if (p.x == 1 && m_state[p.x-1][p.y] == EMPTY)
      {
        moves.emplace_back(p, p.x-1, p.y, B_QUEEN);
        moves.emplace_back(p, p.x-1, p.y, B_ROOK);
        moves.emplace_back(p, p.x-1, p.y, B_BISHOP);
        moves.emplace_back(p, p.x-1, p.y, B_KNIGHT);
      }
      if (p.x == 1 && p.y+1 < 8 && m_state[p.x-1][p.y+1] != EMPTY)
      {
        moves.emplace_back(p, p.x-1, p.y+1, W_QUEEN);
        moves.emplace_back(p, p.x-1, p.y+1, W_ROOK);
        moves.emplace_back(p, p.x-1, p.y+1, W_BISHOP);
        moves.emplace_back(p, p.x-1, p.y+1, W_KNIGHT);
      }
      if (p.x == 1 && p.y >= 1 && m_state[p.x-1][p.y-1] != EMPTY)
      {
        moves.emplace_back(p, p.x-1, p.y-1, W_QUEEN);
        moves.emplace_back(p, p.x-1, p.y-1, W_ROOK);
        moves.emplace_back(p, p.x-1, p.y-1, W_BISHOP);
        moves.emplace_back(p, p.x-1, p.y-1, W_KNIGHT);
      }
      break;
    case W_KNIGHT:
    case B_KNIGHT:
      if(p.x+1 < 8 && p.y+2 < 8)
        moves.emplace_back(p, p.x+1, p.y+2);
      if(p.x+2 < 8 && p.y+1 < 8)
        moves.emplace_back(p, p.x+2, p.y+1);
      if(p.x+1 < 8 && p.y >= 2)
        moves.emplace_back(p, p.x+1, p.y-2);
      if(p.x+2 < 8 && p.y >= 1)
        moves.emplace_back(p, p.x+2, p.y-1);
      if(p.x >= 1 && p.y >= 2)
        moves.emplace_back(p, p.x-1, p.y-2);
      if(p.x >= 2 && p.y >= 1)
        moves.emplace_back(p, p.x-2, p.y-1);
      if(p.x >= 1 && p.y+2 < 8)
        moves.emplace_back(p, p.x-1, p.y+2);
      if(p.x >= 2 && p.y+1 < 8)
        moves.emplace_back(p, p.x-2, p.y+1);
      break;
    case W_BISHOP:
    case B_BISHOP:
      for (int i = p.x+1, j = p.y+1 ; i < 8 && j < 8 ; ++ i, ++ j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x+1, j = p.y-1 ; i < 8 && j >= 0 ; ++ i, -- j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x-1, j = p.y+1 ; i >= 0 && j < 8 ; -- i, ++ j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x-1, j = p.y-1 ; i >= 0 && j >= 0 ; -- i, -- j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      break;
    case W_ROOK:
    case B_ROOK:
      for (int i = p.x+1, j = p.y ; i < 8 ; ++ i) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x, j = p.y-1 ; j >= 0 ; -- j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x, j = p.y+1 ; j < 8 ; ++ j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x-1, j = p.y ; i >= 0 ; -- i) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      break;
    case W_QUEEN:
    case B_QUEEN:
      for (int i = p.x+1, j = p.y+1 ; i < 8 && j < 8 ; ++ i, ++ j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x+1, j = p.y-1 ; i < 8 && j >= 0 ; ++ i, -- j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x-1, j = p.y+1 ; i >= 0 && j < 8 ; -- i, ++ j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x-1, j = p.y-1 ; i >= 0 && j >= 0 ; -- i, -- j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x+1, j = p.y ; i < 8 ; ++ i) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x, j = p.y-1 ; j >= 0 ; -- j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x, j = p.y+1 ; j < 8 ; ++ j) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      for (int i = p.x-1, j = p.y ; i >= 0 ; -- i) {
        moves.emplace_back(p, i, j);
        if (m_state[i][j] != EMPTY)
          break;
      }
      break;
    case W_KING:
    case B_KING:
      if(p.x+1 < 8 && p.y+1 < 8)
        moves.emplace_back(p, p.x+1, p.y+1);
      if(p.x+1 < 8 && p.y < 8)
        moves.emplace_back(p, p.x+1, p.y);
      if(p.x+1 < 8 && p.y >= 1)
        moves.emplace_back(p, p.x+1, p.y-1);
      if(p.x < 8 && p.y+1 < 8)
        moves.emplace_back(p, p.x, p.y+1);
      if(p.x < 8 && p.y >= 1)
        moves.emplace_back(p, p.x, p.y-1);
      if(p.x >= 1 && p.y+1 < 8)
        moves.emplace_back(p, p.x-1, p.y+1);
      if(p.x >= 1 && p.y < 8)
        moves.emplace_back(p, p.x-1, p.y);
      if(p.x >= 1 && p.y >= 1)
        moves.emplace_back(p, p.x-1, p.y-1);
      break;
    default:
      break;
  }
  return moves;
}

bool board::operator==(const board& b) const {
  return (white_to_play == b.white_to_play) && (m_state == b.m_state);
}

board::board() {
  for (int i = 0 ; i < 8 ; ++ i)
  {
    m_state.push_back(std::vector<p_type>());
  }
  for (int i = 0 ; i < 8 ; ++ i)
  {
    m_state[1].push_back(W_PAWN);
    m_state[6].push_back(B_PAWN);
  }
  m_state[0].push_back(W_ROOK);
  m_state[0].push_back(W_KNIGHT);
  m_state[0].push_back(W_BISHOP);
  m_state[0].push_back(W_QUEEN);
  m_state[0].push_back(W_KING);
  m_state[0].push_back(W_BISHOP);
  m_state[0].push_back(W_KNIGHT);
  m_state[0].push_back(W_ROOK);
  m_state[7].push_back(B_ROOK);
  m_state[7].push_back(B_KNIGHT);
  m_state[7].push_back(B_BISHOP);
  m_state[7].push_back(B_QUEEN);
  m_state[7].push_back(B_KING);
  m_state[7].push_back(B_BISHOP);
  m_state[7].push_back(B_KNIGHT);
  m_state[7].push_back(B_ROOK);
  for (int i = 2 ; i < 6 ; ++ i )
  {
    for (int j = 0 ; j < 8 ; ++ j )
    {
      m_state[i].push_back(EMPTY);
    }
  }
}


void board::print_board() {
  std::cout << "displaying board:" << std::endl;
  for (int i = 7 ; i >= 0 ; -- i )
  {
    std::cout << "+";
    for (int c = 0 ; c < 8 ; ++ c )
      std::cout << "---+";
    std::cout << std::endl;
    std::cout << "|";
    for (int j = 0 ; j < 8 ; ++ j )
    {
      switch(m_state[i][j]) {
        case W_PAWN:
        case B_PAWN:
          std::cout << " P |";
          break;
        case W_KNIGHT:
        case B_KNIGHT:
          std::cout << " N |";
          break;
        case W_BISHOP:
        case B_BISHOP:
          std::cout << " B |";
          break;
        case W_ROOK:
        case B_ROOK:
          std::cout << " R |";
          break;
        case W_QUEEN:
        case B_QUEEN:
          std::cout << " Q |";
          break;
        case W_KING:
        case B_KING:
          std::cout << " K |";
          break;
        case EMPTY:
        default:
          std::cout << "   |";
          break;
      }
    }
    std::cout << std::endl;
  }
  std::cout << "+";
  for (int c = 0 ; c < 8 ; ++ c )
    std::cout << "---+";
  std::cout << std::endl;
  std::cout << (white_to_play?"white":"black") << " to play" << std::endl;
}
