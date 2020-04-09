#include <sstream>
// #include <iostream> // for conflict resolution output

#include "move.h"

std::string move::to_str(std::vector<move>& moves) {
  bool duplicate = false;
  std::stringstream conflicting;
  for (auto m : moves)
  {
    if (m.destination_x == destination_x && m.destination_y == destination_y && m.m_piece != m_piece && m.m_piece.type == m_piece.type && m_piece.type != W_PAWN && m_piece.type != B_PAWN)
    {
      duplicate = true;
      if (m.m_piece.y != m_piece.y) {
        conflicting << (char)(m_piece.y + 'a');
      } else {
        conflicting << (char)(m_piece.x + '1');
      }
      /*
      std::cout << "duplicate found for move " << m.destination_x << "," << m.destination_y
                << " and " << destination_x << "," << destination_y
                << " with pieces " << m.m_piece.x << "," << m.m_piece.y << " of type " << m.m_piece.type
                << " and piece " << m_piece.x << "," << m_piece.y << " of type " << m.m_piece.type
                << " resolved conflicting " << conflicting.str()
                << std::endl;
      //*/
    }
  }
  std::stringstream msg;
  msg << piece_id(m_piece.type)
      << (duplicate?conflicting.str():"")
      << (capture?"x":"")
      << case_id(destination_x, destination_y)
      << (promotion==EMPTY?"":"="+piece_id(promotion))
      << (checkmate?"#":(check?"+":""));
  return msg.str();
}

std::string move::case_id(uint8 row, uint8 col) {
  std::stringstream ss;
  ss << (char)(col + 'a');
  ss << (char)(row + '1');
  return ss.str();
}

std::string move::piece_id(p_type t) {
  switch(t) {
    case W_PAWN:
    case B_PAWN:
      if (capture)
      {
        std::stringstream ss;
        ss << (char)(m_piece.y + 'a');
        return ss.str();
      }
      else
        return "";
      break;
    case W_KNIGHT:
    case B_KNIGHT:
      return "N";
      break;
    case W_BISHOP:
    case B_BISHOP:
      return "B";
      break;
    case W_ROOK:
    case B_ROOK:
      return "R";
      break;
    case W_QUEEN:
    case B_QUEEN:
      return "Q";
      break;
    case W_KING:
    case B_KING:
      return "K";
      break;
    case EMPTY:
    default:
      return " ";
      break;
  }
}
