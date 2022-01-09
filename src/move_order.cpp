//
// Created by Yifei Yao on 12/30/21.
//

#include "move_order.h"
#include "move_gen.h"
#include "eval.h"
#include <vector>
#include <algorithm>
#include <list>

namespace move_order {
  std::list<Move>
  GenerateMoveOrderedChildrenList(const Board &board, unsigned ply,
                                  const std::vector<Move> &prev_pvs) {
    std::vector<std::pair<int, Move>> moves;
    for (Move move: move_gen::GenerateAllMoves(board)) {
      moves.emplace_back(GetStaticMoveValue(move, board), move);
    }
    std::sort(moves.begin(), moves.end(), [](auto &lhs, auto &rhs) {
        return lhs.first > rhs.first;
    });
    std::list<Move> sorted_moves;
    for (auto pair: moves) {
      sorted_moves.emplace_back(pair.second);
    }
    if (ply < prev_pvs.size()) {
      Move pv = prev_pvs[ply];
      for (auto child = sorted_moves.begin();
           child != sorted_moves.end(); ++child) {
        if (*child == pv) {
          sorted_moves.erase(child);
          sorted_moves.push_front(*child);
          break;
        }
      }
    }
    return sorted_moves;
  }

  int GetStaticMoveValue(Move move, const Board &pre_move_board) {
    int move_value_estimate = 0;
    Piece::Type target_piece_type = pre_move_board.GetPieceTypeAt(move.to);
    if (target_piece_type != Piece::Type::kNone) {
      int capture_piece_value = eval::GetPieceWeight(target_piece_type);
      move_value_estimate +=
              6 * capture_piece_value - eval::GetPieceWeight(
                      pre_move_board.GetPieceTypeAt(move.from
                      ));
    }
    return move_value_estimate;
  }

  std::vector<Move> GenerateMoveOrderedCaptureOnlyList(const Board &board) {
    std::vector<std::pair<int, Move>> moves;
    for (Move move: move_gen::GenerateCaptureOnly(board)) {
      moves.emplace_back(GetStaticMoveValue(move, board), move);
    }
    std::sort(moves.begin(), moves.end(), [](auto &lhs, auto &rhs) {
        return lhs.first > rhs.first;
    });
    std::vector<Move> sorted_moves;
    sorted_moves.reserve(moves.size());
    for (auto pair: moves) {
      sorted_moves.push_back(pair.second);
    }
    return sorted_moves;
  }
}
