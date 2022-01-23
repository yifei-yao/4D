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

    if (egPhase >= 18) {
      int regular_score = (mgScore * mgPhase + egScore * egPhase) / 24;
      if (regular_score <= 0) {
        return regular_score; // only try force checkmate when winning
      }
      int checkmate_phase = egPhase - 17;
      int checkmate_score = 10 * ForceCheckMateEval(board);
      int final_score = (regular_score * (7 - checkmate_phase) +
                         checkmate_score * checkmate_phase) / 7;
//      std::cout << "Force Check Mate Eval: " << "Regular Score: "
//                << regular_score << "  CheckMate_Score: " << checkmate_score
//                << " Final Score: " << final_score << "\n";
      return final_score;
    } else {
      return (mgScore * mgPhase + egScore * egPhase) / 24;
    }
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

  int ForceCheckMateEval(const Board &board) {
    Position friendly_king, opponent_king;
    for (Position pos = {}; pos.IsInRange(); ++pos) {
      if (board.GetPieceTypeAt(pos) == Piece::Type::kKing) {
        if (board.IsFriendlyAt(pos)) {
          friendly_king = pos;
        } else {
          opponent_king = pos;
        }
      }
    }
    return 3 * CalculateKingDanger(opponent_king) -
           CalculateTwoKingDistance(friendly_king, opponent_king);
  }

  int CalculateKingDanger(Position king_pos) {
    int east_west_bound = std::min(king_pos.col, 7 - king_pos.col);
    int north_south_bound = std::min(king_pos.row, 7 - king_pos.row);
    return 6 - (east_west_bound + north_south_bound);
  }

  int CalculateTwoKingDistance(Position king1, Position king2) {
    int horizontal_dis = abs(king1.col - king2.col);
    int vertical_dis = abs(king1.row - king2.row);
    return horizontal_dis + vertical_dis;
  }
}
