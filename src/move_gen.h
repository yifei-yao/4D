//
// Created by Yifei Yao on 12/24/21.
//

#ifndef INC_4D_MOVE_GEN_H
#define INC_4D_MOVE_GEN_H

#include <vector>
#include "move.h"
#include "board.h"
#include "position.h"

namespace move_gen {
  namespace direction {
    static const Position kNorth = {-1, 0};
    static const Position kSouth = {1, 0};
    static const Position kWest = {0, -1};
    static const Position kEast = {0, 1};
    static const Position kNorthWest = {-1, -1};
    static const Position kNorthEast = {-1, 1};
    static const Position kSouthWest = {1, -1};
    static const Position kSouthEast = {1, 1};

    static const std::array<Position, 8> kAllDirections = {kNorth, kSouth,
                                                           kWest, kEast,
                                                           kNorthWest,
                                                           kNorthEast,
                                                           kSouthWest,
                                                           kSouthEast};
  }

  std::vector<Move> GenerateCaptureOnly(const Board &board);

  void GenerateKingCaptureMoves(const Board &board, Position from,
                                std::vector<Move> &moves);

  bool IsHasNoValidMove(const Board &board);

  bool IsInCheck(const Board &board, Position king_pos);

  void ValidatePush(const Board &board, std::vector<Move> &moves, Move move,
                    Position king_pos);

  std::vector<Move> GenerateAllMoves(const Board &board);

  Position FindKingPos(const Board &board);

  void GenerateKingMoves(const Board &board, Position from,
                         std::vector<Move> &moves);

  void GenerateQueenMoves(const Board &board, Position from,
                          std::vector<Move> &moves, Position king_pos);

  void GenerateQueenCaptureMoves(const Board &board, Position from,
                                 std::vector<Move> &moves, Position king_pos);

  void GenerateRookMoves(const Board &board, Position from,
                         std::vector<Move> &moves, Position king_pos);

  void GenerateRookCaptureMoves(const Board &board, Position from,
                                std::vector<Move> &moves, Position king_pos);

  void GenerateBishopMoves(const Board &board, Position from,
                           std::vector<Move> &moves, Position king_pos);

  void GenerateBishopCaptureMoves(const Board &board, Position from,
                                  std::vector<Move> &moves, Position king_pos);

  void GenerateKnightMoves(const Board &board, Position from,
                           std::vector<Move> &moves, Position king_pos);

  void GenerateKnightCaptureMoves(const Board &board, Position from,
                                  std::vector<Move> &moves, Position king_pos);

  void GeneratePawnMoves(const Board &board, Position from,
                         std::vector<Move> &moves, Position king_pos);

  void GeneratePawnCaptureMoves(const Board &board, Position from,
                                std::vector<Move> &moves, Position king_pos);

  void GenerateCastlingMoves(const Board &board, std::vector<Move> &moves,
                             Position king_pos);
}


#endif //INC_4D_MOVE_GEN_H
