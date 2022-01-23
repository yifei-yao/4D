//
// Created by Yifei Yao on 1/2/22.
//

#include "uci.h"
#include "move.h"
#include "move_gen.h"
#include "search.h"
#include "game.h"
#include <sstream>
#include <map>
#include <vector>

void uci::UCIMainLoop() {
  std::cout << "id name 4D" << std::endl;
  std::cout << "id author Yifei Yao" << std::endl;
  game::Game game;
  std::cout << "uciok" << std::endl;
  TimeController tc;
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss;
    ss << line;
    std::string first_word;
    ss >> first_word;
    if (first_word == "exit" || first_word == "quit" || first_word == "q") {
      break;
    }
    if (first_word == "isready") {
      std::cout << "readyok" << std::endl;
    }
    if (first_word == "ucinewgame") {
      game = game::Game();
    }
    if (first_word == "stop") {
      tc.StopNow();
    }
    if (first_word == "go") {
      tc.SetTime(3);
      Move move = search::IDRoot(game.GetCurrentBoard(), tc,
                                 game.GetUniquePos(), game.GetTwofoldRepeat());
      std::cout << "bestmove " << move << std::endl;
      game.ApplyMove(move);
    }
    if (first_word == "position") {
      std::string second_word;
      ss >> second_word;
      std::string third_word;
      if (!second_word.empty()) {
        if (second_word == "startpos") {
          game = game::Game();
          ss >> third_word;
        } else {
          std::vector<std::string> fen_words;
          fen_words.push_back(second_word);
          std::string new_word;
          while (ss >> new_word) {
            if (new_word == "moves") {
              third_word = new_word;
              break;
            }
            fen_words.push_back(new_word);
          }
          std::stringstream ss_fen;
          for (const std::string &word: fen_words) {
            ss_fen << word << " ";
          }
          game = game::Game(ss_fen.str());
        }
        if (third_word == "moves") {
          std::string str_move;
          while (ss >> str_move) {
            std::map<std::string, Move> valid_moves;
            for (Move move: move_gen::GenerateAllMoves(
                    game.GetCurrentBoard())) {
              valid_moves.insert({move.GetNotation(), move});
            }
            if (valid_moves.find(str_move) != valid_moves.end()) {
              Move move = valid_moves.find(str_move)->second;
              game.ApplyMove(move);
            } else {
              break;
            }
          }
        }
      }
    }
  }
}
