#include "piece.h"

piece& piece::operator=(const piece& p)
{
  x = p.x;
  y = p.y;
  type = p.type;
}

bool piece::operator==(const piece& rhs) const
{
  return rhs.x == x && rhs.y == y && rhs.type == type;
}

bool piece::operator!=(const piece& rhs) const
{
  return rhs.x != x || rhs.y != y || rhs.type != type;
}
