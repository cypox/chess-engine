#include <iomanip> // for std::setfill and std::setw
#include <thread> // for this_thread::sleep_for
#include <chrono> // for chrono::milliseconds
#include <sstream> // for filename
#include <fstream> // for writing moves
#include <iostream>

#include "engine.h"
#include "board.h"
#include "move.h"

int main(int argc, char** argv)
{
  unsigned int seed = time(nullptr);
  //unsigned int seed = 1586417037;
  srand(seed);
  board x;
  engine e(x);

  int move_number = 1;
  std::cout << "board loaded successfully with seed " << seed << std::endl;
  x.print_board();
  std::stringstream filename;
  filename << "games/" << seed << ".pgn";
  std::ofstream log(filename.str(), std::ofstream::out);
  log << "[Event \"Testing matches\"]" << std::endl;
  log << "[Site \"Valenciennes, France\"]" << std::endl;
  log << "[Date \"2020.09.04\"]" << std::endl;
  log << "[Round \"-\"]" << std::endl;
  log << "[White \"cypox\"]" << std::endl;
  log << "[Black \"engine-v0\"]" << std::endl;
  log << "[Result \"1/2-1/2\"]" << std::endl;
  log << std::endl;
  while(true)
  {
    /*
    if (move_number == 72)
      std::cout << "debugging";
    */
    //std::vector<move> moves = x.get_possible_moves(x.to_play());
    std::vector<move> moves = e.ponder();
    if (moves.size() == 0)
    {
      std::cout << "\033[F" << "checkmate (or stalemate)! " << (x.to_play()?"black":"white") << " won (or draw)." << std::endl;
      break;
    }
    /*
    int i = 0;
    for (auto s : moves)
    {
      std::cout << "["  << std::setfill('0') << std::setw(2)<< ++ i << "] " << s.to_str(moves) << " (" << s.get_score() << ")     " << std::endl;
    }
    getchar();std::cout << "\033[F";
    //*/
    //int choice = rand() % moves.size();
    move mv;
    if (x.to_play() == true)
      mv = moves.front();
    else
      mv = moves.back();
    std::string move_str = mv.to_str(moves);

    std::cout << "move: " << move_str << "     " << std::endl;
    if (x.to_play())
      log << move_number << ". " << move_str << " " << std::flush;
    else
    {
      log << move_str << " " << std::flush;
      ++ move_number;
    }

    for (int l = 0 ; l < 20 ; ++ l)
      //*
      std::cout << "\033[F";
      //*/;
    x.play_unsafe(mv);
    x.print_board();
    e.update_board(x);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  log << "1/2-1/2";
  log.close();
  return 0;
}
