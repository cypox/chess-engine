#include <iomanip> // for std::setfill and std::setw
#include <thread> // for this_thread::sleep_for
#include <chrono> // for chrono::milliseconds
#include <fstream> // for writing moves
#include <iostream>

#include "board.h"
#include "move.h"

int main(int argc, char** argv)
{
  //srand(time(null));
  board x;
  int move_number = 1;
  std::cout << "board loaded successfully" << std::endl;
  x.print_board();
  std::ofstream log("moves", std::ofstream::out);
  while(true)
  {
    std::vector<move> moves = x.get_possible_moves(x.to_play());
    if (moves.size() == 0)
    {
      std::cout << "\033[F" << "checkmate (or stalemate)! " << (x.to_play()?"black":"white") << " won (or draw)." << std::endl;
      break;
    }
    //*
    int i = 0;
    for (auto s : moves)
    {
      std::cout << "["  << std::setfill('0') << std::setw(2)<< ++ i << "] " << s.to_str() << "     " << std::endl;
    }
    getchar();std::cout << "\033[F";
    //*/
    int choice = rand() % moves.size();

    std::cout << "move: " << moves[choice].to_str() << "     " << std::endl;
    if (x.to_play())
      log << move_number << ". " << moves[choice].to_str();
    else
    {
      log << " " << moves[choice].to_str() << std::endl;
      ++ move_number;
    }

    for (int l = 0 ; l < 20 ; ++ l)
      std::cout << "\033[F";
    x.play_unsafe(moves[choice]);
    x.print_board();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  log.close();
  return 0;
}
