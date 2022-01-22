//
// Created by Yifei Yao on 12/26/21.
//

#include "game.h"
#include "move_gen.h"
#include <map>
#include "search.h"
#include "zobrist.h"

namespace game {
  void HumanVSAILoop(const std::string &fen_notation) {
    do {
      color::Color player_color = PickStartingColor();
      Board board(fen_notation);
      board.Print(player_color);
      do {
        if (board.GetColor() == player_color) {
          Move move = AskPlayerMove(board);
          board = Board(board, move);
        } else {
          Move move = search::IDRoot(board, TimeController(4));
          std::cout << "AI move: " << move << "\n";
          board = Board(board, move);
        }
        board.Print(player_color);
      } while (!IsGameEnded(board, player_color));
    } while (AskPlayAgain());
  }

  bool IsGameEnded(const Board &board, color::Color player_color) {
    if (!move_gen::IsCheckMated(board)) {
      return false;
    } else {
      std::cout << "Game Over\n";
      if (board.GetColor() == player_color) {
        std::cout << "You LOST!\n";
      } else {
        std::cout << "You WON!\n";
      }
      return true;
    }
  }

  bool AskPlayAgain() {
    while (true) {
      std::cout << "Play again? (y/n)\n";
      char input;
      std::cin >> input;
      switch (input) {
        case 'y':
        case 'Y':
          return true;
        case 'n':
        case 'N':
          return false;
        default:
          std::cout << "I'll take that as a NO.\n";
          return false;
      }
    }
  }

  color::Color PickStartingColor() {
    while (true) {
      std::cout << "You pick white or black?(w/b)\n";
      std::string choice;
      std::cin >> choice;
      switch (choice[0]) {
        case 'w':
          return color::Color::kWhite;
        case 'b':
          return color::Color::kBlack;
        default:
          break;
      }
    }
  }

  Move AskPlayerMove(const Board &board) {
    std::string input;
    while (true) {
      std::cout << "Your turn. Enter move coordinates(eg. c4d5):\n";
      std::cin >> input;
      std::map<std::string, Move> valid_moves;
      for (Move move: move_gen::GenerateAllMoves(board)) {
        valid_moves.insert({move.GetNotation(), move});
      }
      if (valid_moves.find(input) != valid_moves.end()) {
        return valid_moves.find(input)->second;
      } else {
        std::cout << "Invalid move. Try again\n";
        UserInputHelp();
      }
    }
  }

  void UserInputHelp() {
    std::cout
            << "<move descriptor> ::= <from square><to square>[<promoted to>]\n"
               "<square>          ::= <file letter><rank number>\n"
               "<file letter>     ::= 'a'|'b'|'c'|'d'|'e'|'f'|'g'|'h'\n"
               "<rank number>     ::= '1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'\n"
               "<promoted to>     ::= 'q'|'r'|'b'|'n'\n";
  }

  Game::Game()
          : current_board(), half_move_clock(0),
            turn_to_move(color::Color::kWhite) {
    unique_pos.insert(zobrist::GetHashKey(current_board));
  }
}
