#include <sstream>

#include "move.h"

std::string move::to_str() {
  std::stringstream msg;
  msg << piece_id(m_piece.type) << (capture?"x":"") << case_id(destination_x, destination_y) << (checkmate?"#":(check?"+":""));
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
