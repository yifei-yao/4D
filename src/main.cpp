#include <iostream>
#include "game.h"
#include "uci.h"


int main() {
  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "exit" || command == "quit") {
      break;
    }
    if (command == "play") {
      game::HumanVSAILoop();
    }
    if (command == "uci") {
      uci::UCIMainLoop();
    }
  }
}
