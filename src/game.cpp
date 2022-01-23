//
// Created by Yifei Yao on 12/26/21.
//

#include "game.h"
#include "move_gen.h"
#include "search.h"
#include "zobrist.h"
#include <map>
#include <sstream>
#include <algorithm>

namespace game {
  void HumanVSAILoop(const std::string &fen_notation) {
    do {
      Game game(fen_notation);
      color::Color player_color = PickStartingColor();
      game.Print(player_color);
      do {
        if (game.GetTurnToMove() == player_color) {
          Move move = AskPlayerMove(game.GetCurrentBoard());
          game.ApplyMove(move);
        } else {
          Move move = search::IDRoot(game.GetCurrentBoard(), TimeController(4),
                                     game.GetUniquePos(),
                                     game.GetTwofoldRepeat());
          std::cout << "AI move: " << move << "\n";
          game.ApplyMove(move);
        }
        game.Print(player_color);
      } while (!CheckGameEnded(game, player_color));
    } while (AskPlayAgain());
  }

  bool CheckGameEnded(const Game &game, color::Color player_color) {
    if (game.IsEnded()) {
      std::cout << "Game Over\n";
      if (game.GetGameState() == Game::GameState::kDraw) {
        std::cout << "DRAW!\n";
      } else {
        color::Color winner;
        if (game.GetGameState() == Game::GameState::kWhiteWon) {
          winner = color::Color::kWhite;
        } else {
          winner = color::Color::kBlack;
        }
        if (winner == player_color) {
          std::cout << "You WON!\n";
        } else {
          std::cout << "You LOST!\n";
        }
      }
      return true;
    } else {
      return false;
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
            turn_to_move(color::Color::kWhite), state(GameState::kOngoing),
            move_count(0) {
    unique_pos.insert(zobrist::GetHashKey(current_board));
  }

  Game::Game(const std::string &fen_notation) : move_count(0) {
    std::stringstream ss;
    std::string head, castling, en_passant;
    char color_char;
    unsigned half_move;
    ss << fen_notation;
    if (!(ss >> head >> color_char >> castling >> en_passant >> half_move) ||
        half_move > 100) {
      std::cerr << "Invalid FEN notation format!\n";
      current_board = Board();
      half_move_clock = 0;
      turn_to_move = color::Color::kWhite;
      unique_pos.insert(zobrist::GetHashKey(current_board));
      state = GameState::kOngoing;
      return;
    }
    std::stringstream main_fen;
    main_fen << head << " " << color_char << " " << castling << " "
             << en_passant;
    current_board = Board(main_fen.str());
    unique_pos.insert(zobrist::GetHashKey(current_board));
    // FEN notation cannot check threefold repetition
    half_move_clock = half_move;
    turn_to_move = color::FenToColor(color_char);
    state = GameState::kOngoing;
  }

  bool Game::IsEnded() const {
    return GetGameState() != GameState::kOngoing;
  }

  Game::GameState Game::GetGameState() const {
    return state;
  }

  void Game::ApplyMove(Move valid_move) {
    // assume valid move; for user input, must be validated beforehand
    if (IsEnded()) {
      throw std::logic_error("Cannot make move after game termination");
    }

    std::vector<Move> all_moves = move_gen::GenerateAllMoves(current_board);
    if (std::find(all_moves.begin(), all_moves.end(), valid_move) ==
        all_moves.end()) {
      throw std::range_error("Attempt to apply illegal move");
    }

    if (GetTurnToMove() == color::Color::kBlack) {
      ++move_count;
    }

    bool is_capture_or_pawn_move = false;
    if (current_board.GetPieceTypeAt(valid_move.to) != Piece::Type::kNone ||
        current_board.GetPieceTypeAt(valid_move.from) ==
        Piece::Type::kPawn) {
      is_capture_or_pawn_move = true;
    }

    if (is_capture_or_pawn_move) {
      half_move_clock = 0;
    } else {
      ++half_move_clock;
    }

    current_board = Board(current_board, valid_move);

    turn_to_move = !turn_to_move;

    if (move_gen::IsHasNoValidMove(current_board)) {
      if (move_gen::IsInCheck(current_board,
                              move_gen::FindKingPos(current_board))) {
        switch (turn_to_move) {
          case color::Color::kWhite:
            state = GameState::kBlackWon;
            return;
          case color::Color::kBlack:
            state = GameState::kWhiteWon;
            return;
          case color::Color::kNone:
            throw std::logic_error(
                    "No one won but game ended and is not a draw");
        }
      } else {
        state = GameState::kDraw; // stalemate
        return;
      }
    }

    uint64_t hashed_pos = zobrist::GetHashKey(current_board);

    if (twofold_repeat_pos.find(hashed_pos) != twofold_repeat_pos.end()) {
      state = GameState::kDraw;
      return;
    }
    auto found = unique_pos.find(hashed_pos);
    if (found != unique_pos.end()) {
      unique_pos.erase(found);
      twofold_repeat_pos.insert(hashed_pos);
    } else {
      unique_pos.insert(hashed_pos);
    }

    if (half_move_clock == 100) {
      state = GameState::kDraw;
      return;
    }
  }

  void Game::Print(color::Color perspective) const {
    current_board.Print(perspective);
  }

  color::Color Game::GetTurnToMove() const {
    return turn_to_move;
  }

  const Board &Game::GetCurrentBoard() const {
    return current_board;
  }

  const std::set<uint64_t> &Game::GetTwofoldRepeat() const {
    return twofold_repeat_pos;
  }

  const std::set<uint64_t> &Game::GetUniquePos() const {
    return unique_pos;
  }

  unsigned Game::GetMoveCount() const {
    return move_count;
  }
}
