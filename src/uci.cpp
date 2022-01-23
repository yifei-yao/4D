//
// Created by Yifei Yao on 1/2/22.
//

#include "uci.h"
#include "move.h"
#include "move_gen.h"
#include "search.h"
#include "game.h"
#include "eval.h"
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
      double time = GetTime(ParseGo(ss), game.GetTurnToMove(),
                            game.GetCurrentBoard());
      tc.SetTime(time);
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

std::map<std::string, double> uci::ParseGo(std::stringstream &ss) {
  std::string command;
  double value;
  std::map<std::string, double> options;
  while (ss >> command) {
    if (ss >> value) {
      options.insert({command, value});
    }
  }
  return options;
}

double uci::GetTime(const std::map<std::string, double> &options,
                    color::Color color_to_move, const Board &board) {
  if (options.empty()) {
    return 10;
  }
  std::string time_arg, incr_arg;
  switch (color_to_move) {
    case color::Color::kWhite: {
      time_arg = "wtime";
      incr_arg = "winc";
      break;
    }
    case color::Color::kBlack: {
      time_arg = "btime";
      incr_arg = "binc";
      break;
    }
    case color::Color::kNone:
      throw std::logic_error("Must have a color to move");
  }
  double total_time;
  if (options.find(time_arg) != options.end()) {
    total_time = options.find(time_arg)->second;
  } else {
    total_time = 0;
  }
  double incr_time;
  if (options.find(incr_arg) != options.end()) {
    incr_time = options.find(incr_arg)->second;
  } else {
    incr_time = 0;
  }
  double moves_to_go;
  if (options.find("movestogo") != options.end()) {
    moves_to_go = options.find("movestogo")->second;
  } else {
    double progress = eval::CalculateGameProgress(board);
    moves_to_go = 48 - 43 * progress;
  }
  double time = total_time / moves_to_go + incr_time;
  return time / 1000.0;
}
