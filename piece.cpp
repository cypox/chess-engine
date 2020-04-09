#include "piece.h"

piece& piece::operator=(const piece& p)
{
  x = p.x;
  y = p.y;
  type = p.type;
}
