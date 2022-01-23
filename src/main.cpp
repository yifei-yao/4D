#include <iostream>
#include "game.h"
#include "uci.h"


int main() {
//  auto my_game = game::Game("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");

  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "exit" || command == "quit") {
      break;
    }
    if (command == "play") {
      game::HumanVSAILoop("8/P7/4k3/1r6/8/8/7K/8 w - - 0 69");
    }
    if (command == "uci") {
      uci::UCIMainLoop();
    }
  }
}
