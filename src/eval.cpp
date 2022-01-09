//
// Created by Yifei Yao on 12/31/21.
//

#include "eval.h"

namespace eval {
  int Flip(int sq) {
    return sq ^ 56;
  }

  int Evaluate(const Board &board) {
    static std::array<std::array<int, 64>, 12> mg_table = InitMgTable();
    static std::array<std::array<int, 64>, 12> eg_table = InitEgTable();

    std::array<int, 2> mg{};
    std::array<int, 2> eg{};
    int gamePhase = 0;

    for (int sq = 0; sq < 64; ++sq) {
      Piece piece = board.GetPieceAt(sq);
      if (!piece.IsNone()) {
        color::Color color = piece.GetColor();
        int pc_color = PCOLOR(color);
        int pc = PC(piece);
        mg[pc_color] += mg_table[pc][sq];
        eg[pc_color] += eg_table[pc][sq];
        gamePhase += game_phase_inc[pc];
      }
    }
    int side_to_move = PCOLOR(board.GetColor());

    int mgScore = mg[side_to_move] - mg[Other(side_to_move)];
    int egScore = eg[side_to_move] - eg[Other(side_to_move)];
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;
    return (mgScore * mgPhase + egScore * egPhase) / 24;
  }

  std::array<std::array<int, 64>, 12> InitMgTable() {
    static std::array<std::array<int, 64>, 12> mg_table;
    for (int i = 0; i < 6; ++i) {
      for (int sq = 0; sq < 64; ++sq) {
        mg_table[2 * i][sq] = mg_value[i] + mg_pesto_table[i][sq];
        mg_table[2 * i + 1][sq] = mg_value[i] + mg_pesto_table[i][Flip(sq)];
      }
    }
    return mg_table;
  }

  std::array<std::array<int, 64>, 12> InitEgTable() {
    static std::array<std::array<int, 64>, 12> eg_table;
    for (int i = 0; i < 6; ++i) {
      for (int sq = 0; sq < 64; ++sq) {
        eg_table[2 * i][sq] = eg_value[i] + eg_pesto_table[i][sq];
        eg_table[2 * i + 1][sq] = eg_value[i] + eg_pesto_table[i][Flip(sq)];
      }
    }
    return eg_table;
  }

  int PCOLOR(color::Color color) {
    switch (color) {
      case color::Color::kWhite:
        return 0;
      case color::Color::kBlack:
        return 1;
      case color::Color::kNone:
        throw std::logic_error("PCOLOR cannot be from kNone");
    }
  }

  int PC(Piece piece) {
    int color = PCOLOR(piece.GetColor());
    switch (piece.GetType()) {
      case Piece::Type::kPawn:
        return 0 + color;
      case Piece::Type::kKnight:
        return 2 + color;
      case Piece::Type::kBishop:
        return 4 + color;
      case Piece::Type::kRook:
        return 6 + color;
      case Piece::Type::kQueen:
        return 8 + color;
      case Piece::Type::kKing:
        return 10 + color;
      case Piece::Type::kNone:
        throw std::logic_error("Does not process kNone value");
    }
  }

  int Other(int side) {
    return side ^ 1;
  }

  int GetPieceWeight(Piece::Type piece_type) {
    switch (piece_type) {
      case Piece::Type::kQueen:
        return 980;
      case Piece::Type::kKnight:
        return 309;
      case Piece::Type::kRook:
        return 495;
      case Piece::Type::kBishop:
        return 331;
      case Piece::Type::kPawn:
        return 88;
      case Piece::Type::kNone:
      case Piece::Type::kKing:
        return 0;
    }
  }
}
