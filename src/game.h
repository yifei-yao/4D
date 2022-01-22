//
// Created by Yifei Yao on 12/26/21.
//

#ifndef INC_4D_GAME_H
#define INC_4D_GAME_H

#include "board.h"
#include "color.h"
#include <iostream>
#include <set>

namespace game {
  class Game;

  void HumanVSAILoop(
          const std::string &fen_notation = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  color::Color PickStartingColor();

  bool AskPlayAgain();

  bool CheckGameEnded(const Game &game, color::Color player_color);

  Move AskPlayerMove(const Board &board);

  void UserInputHelp();

  class Game {
  public:
    Game();

    explicit Game(const std::string &fen_notation);

    enum class GameState {
      kOngoing, kWhiteWon, kBlackWon, kDraw,
    };

    [[nodiscard]] color::Color GetTurnToMove() const;

    [[nodiscard]] const Board &GetCurrentBoard() const;

    void Print(color::Color perspective) const;

    void ApplyMove(Move valid_move);

    [[nodiscard]] GameState GetGameState() const;

    [[nodiscard]] bool IsEnded() const;

    [[nodiscard]] const std::set<uint64_t> &GetTwofoldRepeat() const;

  private:
    unsigned half_move_clock;
    std::set<uint64_t> unique_pos;
    std::set<uint64_t> twofold_repeat_pos;
    color::Color turn_to_move;
    Board current_board;
    GameState state;
  };
}
#endif //INC_4D_GAME_H
