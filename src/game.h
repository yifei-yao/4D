//
// Created by Yifei Yao on 12/26/21.
//

#ifndef INC_4D_GAME_H
#define INC_4D_GAME_H

#include "board.h"
#include "color.h"
#include <iostream>

namespace game {
  void HumanVSAILoop(
          const std::string &fen_notation = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  color::Color PickStartingColor();

  bool AskPlayAgain();

  bool IsGameEnded(const Board &board, color::Color player_color);

  Move AskPlayerMove(const Board &board);

  void UserInputHelp();
}
#endif //INC_4D_GAME_H
