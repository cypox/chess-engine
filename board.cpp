#include "board.h"

bool board::will_check(move& m) const {
  piece opponent_king;
  std::vector<piece> opponent_pieces = get_player_pieces(!white_to_play);
  for (auto p : opponent_pieces)
  {
    if (p.type == W_KING || p.type == B_KING)
      opponent_king = p;
  }
  std::vector<move> moves = get_moves_after_simulation(white_to_play, m);
  for (auto p_move : moves)
  {
    if (p_move.destination_x == opponent_king.x && p_move.destination_y == opponent_king.y)
      return true;
  }
  return false;
}

bool board::will_be_in_check(move& m) const {
  piece king;
  if (m.m_piece.type == W_KING || m.m_piece.type == B_KING)
  {
    king = piece(m.destination_x, m.destination_y, m.m_piece.type);
  }
  else
  {
    std::vector<piece> pieces = get_player_pieces(white_to_play);
    for (auto p : pieces)
    {
      if (p.type == W_KING || p.type == B_KING)
        king = p;
    }
  }
  std::vector<move> opponent_moves = get_all_moves_with_checks(!white_to_play);
  for (auto opponent_m : opponent_moves)
  {
    if (opponent_m.m_piece.x == m.destination_x && opponent_m.m_piece.y == m.destination_y)
      continue; // this piece was captured by move m
    if (opponent_m.destination_x == king.x && opponent_m.destination_y == king.y)
      return true;
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

std::vector<piece> board::get_player_pieces(bool player) const {
  std::vector<piece> pieces;
  uint8 range_min = player?1:7;
  uint8 range_max = player?6:12;
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
  possible_move &= !will_be_in_check(m);
  m.set_check(will_check(m));
  return possible_move;
}

std::vector<move> board::get_possible_moves(bool player) const {
  std::vector<move> moves;
  std::vector<piece> pieces = get_player_pieces(player);
  for (auto p:pieces) {
    std::vector<move> possible = get_piece_moves(p);
    for (auto m:possible) {
      if (process_move(m))
        moves.push_back(m);
    }
  }
  return moves;
}

std::vector<move> board::get_all_moves_with_checks(bool player) const {
  std::vector<move> moves;
  std::vector<piece> pieces = get_player_pieces(player);
  for (auto p:pieces) {
    std::vector<move> possible = get_piece_moves(p);
    for (auto m:possible) {
      if (!is_collision(m))
        moves.push_back(m);
    }
  }
  return moves;
}

std::vector<move> board::get_moves_after_simulation(bool player, move& m) const {
  std::vector<move> moves;
  std::vector<piece> pieces = get_player_pieces(player);
  for (auto p:pieces) {
    // move piece if concerned
    if (p.x == m.m_piece.x && p.y == m.m_piece.y)
    {
      p.x = m.destination_x;
      p.y = m.destination_y;
    }
    std::vector<move> possible = get_piece_moves(p);
    for (auto m:possible) {
      if (!is_collision(m))
        moves.push_back(m);
    }
  }
  return moves;
}

void board::play_unsafe(move& m) {
  m_state[m.destination_x][m.destination_y] = m.m_piece.type;
  m_state[m.m_piece.x][m.m_piece.y] = EMPTY;
  switch_player();
}

std::vector<move> board::get_piece_moves(piece& p) const {
  std::vector<move> moves;
  switch (p.type) {
    case W_PAWN:
      if (p.x+1 < 8 && m_state[p.x+1][p.y] == EMPTY)
        moves.emplace_back(p, p.x+1, p.y);
      if (p.x < 2 && m_state[p.x+2][p.y] == EMPTY && m_state[p.x+1][p.y] == EMPTY)
        moves.emplace_back(p, p.x+2, p.y);
      if (p.x+1 < 8 && p.y+1 < 8 && m_state[p.x+1][p.y+1] != EMPTY)
        moves.emplace_back(p, p.x+1, p.y+1);
      if (p.x+1 < 8 && p.y >= 1 && m_state[p.x+1][p.y-1] != EMPTY)
        moves.emplace_back(p, p.x+1, p.y-1);
      break;
    case B_PAWN:
      if (p.x >= 1 && m_state[p.x-1][p.y] == EMPTY)
        moves.emplace_back(p, p.x-1, p.y);
      if (p.x >= 6 && m_state[p.x-2][p.y] == EMPTY && m_state[p.x-1][p.y] == EMPTY)
        moves.emplace_back(p, p.x-2, p.y);
      if (p.x >= 1 && p.y+1 < 8 && m_state[p.x-1][p.y+1] != EMPTY)
        moves.emplace_back(p, p.x-1, p.y+1);
      if (p.x >= 1 && p.y >= 1 && m_state[p.x-1][p.y-1] != EMPTY)
        moves.emplace_back(p, p.x-1, p.y-1);
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
      if(p.x < 8 && p.y < 8)
        moves.emplace_back(p, p.x, p.y-1);
      if(p.x >= 1 && p.y+1 <8)
        moves.emplace_back(p, p.x-1, p.y+1);
      if(p.x >= 1 && p.y < 8)
        moves.emplace_back(p, p.x-1, p.y);
      if(p.x >= 1 && p.y+1 >= 1)
        moves.emplace_back(p, p.x-1, p.y-1);
      break;
    default:
      break;
  }
  return moves;
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
