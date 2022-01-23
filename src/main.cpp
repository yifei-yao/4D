#include "game.h"
#include "uci.h"
#include <iostream>

int main() {
  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "exit" || command == "quit" || command == "q") {
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
