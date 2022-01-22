//
// Created by Yifei Yao on 1/2/22.
//

#include "search.h"
#include "eval.h"
#include "move_gen.h"
#include "infinity.h"
#include "move_order.h"

#include <list>
#include <chrono>
#include <map>

namespace search {
  Move IDRoot(const Board &board, TimeController time_controller,
              const std::set<uint64_t> &twofold_repeat_pos) {
    Move pre_move;
    Move curr_move;
    std::vector<Move> prev_pv;
    std::vector<Move> curr_pv;
    curr_pv.emplace_back();
    unsigned depth = 1;


    while (!time_controller.IsTimeUp()) {
      pre_move = curr_move;
      int score;
      auto pair = PVSRoot(board, depth, prev_pv, curr_pv, time_controller,
                          twofold_repeat_pos);
      curr_move = pair.first;
      score = pair.second;
      if (!time_controller.IsTimeUp()) {
        if (score == infinity::kInfinity ||
            score == infinity::kNegativeInfinity) {
          return curr_move;
        }
        prev_pv = curr_pv;
        curr_pv.clear();
        curr_pv.resize(++depth);
      }
    }
    return pre_move;
  }

  std::pair<Move, int> PVSRoot(const Board &board, unsigned int depth,
                               const std::vector<Move> &prev_pv,
                               std::vector<Move> &curr_pv,
                               TimeController time_controller,
                               const std::set<uint64_t> &twofold_repeat_pos) {
    TT table{};

    if (depth < 1) {
      throw std::range_error("Depth must be >= 1");
    }

    int alpha = infinity::kNegativeInfinity;
    int beta = infinity::kInfinity;

    std::list<Move> children = move_order::GenerateMoveOrderedChildrenList(
            board, 0, prev_pv);

    Move best_move = children.front();

    Board best_child = Board(board, best_move);

    int best_score;
    if (move_gen::IsHasNoValidMove(best_child)) {
      if (move_gen::IsInCheck(best_child, move_gen::FindKingPos(best_child))) {
        best_score = infinity::kInfinity;
      } else {
        best_score = 0;
      }
    } else {
      if (IsRepetitionDraw(best_child, twofold_repeat_pos)) {
        best_score = 0;
      } else {
        best_score = -PVS({board, children.front()}, depth - 1,
                          -beta, -alpha,
                          table, 1, prev_pv, curr_pv, time_controller);
      }
    }

    if (best_score > alpha) {
      if (best_score >=
          beta) {
        curr_pv[0] = best_move;
        return {best_move, best_score};
      }
      alpha = best_score;
    }

    for (auto child = ++children.begin(); child != children.end(); ++child) {
      Board child_board(board, *child);

      int score;
      if (move_gen::IsHasNoValidMove(child_board)) {
        if (move_gen::IsInCheck(child_board,
                                move_gen::FindKingPos(child_board))) {
          score = infinity::kInfinity;
        } else {
          score = 0;
        }
      } else {
        if (IsRepetitionDraw(child_board, twofold_repeat_pos)) {
          score = 0;
        } else {
          score = -PVS(child_board, depth - 1, -alpha - 1, -alpha, table, 1,
                       prev_pv, curr_pv, time_controller);
        }
      }
      if (score > alpha && score < beta) {
        score = -PVS(child_board, depth - 1, -beta, -alpha, table, 1, prev_pv,
                     curr_pv, time_controller);
        if (score > alpha) {
          alpha = score;
        }
      }
      if (score > best_score) {
        best_move = *child;
        if (score >= beta) {
          curr_pv[0] = best_move;
          return {best_move, score};
        }
        best_score = score;
      }
    }
    curr_pv[0] = best_move;
    return {best_move, best_score};
  }

  int PVS(const Board &board, unsigned int depth, int alpha, int beta,
          TT &table, unsigned int ply,
          const std::vector<Move> &prev_pv,
          std::vector<Move> &curr_pv, TimeController time_controller) {

    if (time_controller.IsTimeUp()) {
      return infinity::kInfinity;
    }

    int alpha_orig = alpha;

    TT::Answer answer = table.GetEntry(board, ply);

    if (answer.is_success) {
      TT::Value value = answer.value;
      switch (value.type) {
        case TT::Value::Type::kExact:
          return value.score;
        case TT::Value::Type::kLowerBound: {
          if (value.score > alpha) {
            alpha = value.score;
          }
          break;
        }
        case TT::Value::Type::kUpperBound: {
          if (value.score < beta) {
            beta = value.score;
          }
          break;
        }
      }
      if (alpha >= beta) {
        return value.score;
      }
    }

    std::list<Move> children = move_order::GenerateMoveOrderedChildrenList(
            board, ply, prev_pv);

    if (children.empty()) {
      if (move_gen::IsInCheck(board, move_gen::FindKingPos(board))) {
        return infinity::kNegativeInfinity; // checkmate
      } else {
        return 0; // stalemate
      }
    }

    if (depth == 0) {
      int score = Quiesce(board, alpha, beta);
      return score;
    }

    Move best_move = children.front();

    int best_score = -PVS({board, children.front()}, depth - 1, -beta, -alpha,
                          table, ply + 1, prev_pv, curr_pv, time_controller);

    if (best_score > alpha) {
      if (best_score >= beta) {
        if (best_score <= alpha_orig) {
          table.AddEntry(board, {TT::Value::Type::kUpperBound, best_score},
                         ply);
        } else if (best_score >= beta) {
          table.AddEntry(board, {TT::Value::Type::kLowerBound, best_score},
                         ply);
        } else {
          table.AddEntry(board, {TT::Value::Type::kExact, best_score}, ply);
        }
        curr_pv[ply] = best_move;
        return best_score;
      }
      alpha = best_score;
    }

    for (auto child = ++children.begin(); child != children.end(); ++child) {
      Board child_board(board, *child);
      int score = -PVS(child_board, depth - 1, -alpha - 1, -alpha, table,
                       ply + 1, prev_pv, curr_pv, time_controller);
      if (score > alpha && score < beta) {
        score = -PVS(child_board, depth - 1, -beta, -alpha, table, ply + 1,
                     prev_pv, curr_pv, time_controller);
        if (score > alpha) {
          alpha = score;
        }
      }
      if (score > best_score) {
        best_move = *child;
        if (score >= beta) {
          if (score <= alpha_orig) {
            table.AddEntry(board, {TT::Value::Type::kUpperBound, score}, ply);
          } else if (score >= beta) {
            table.AddEntry(board, {TT::Value::Type::kLowerBound, score}, ply);
          } else {
            table.AddEntry(board, {TT::Value::Type::kExact, score}, ply);
          }
          curr_pv[ply] = best_move;
          return score;
        }
        best_score = score;
      }
    }
    if (best_score <= alpha_orig) {
      table.AddEntry(board, {TT::Value::Type::kUpperBound, best_score}, ply);
    } else if (best_score >= beta) {
      table.AddEntry(board, {TT::Value::Type::kLowerBound, best_score}, ply);
    } else {
      table.AddEntry(board, {TT::Value::Type::kExact, best_score}, ply);
    }
    curr_pv[ply] = best_move;
    return best_score;
  }

  int Quiesce(const Board &board, int alpha, int beta) {

    int stand_pat = eval::Evaluate(board);
    if (stand_pat >= beta) {
      return beta;
    }
    if (alpha < stand_pat) {
      alpha = stand_pat;
    }

    std::vector<Move> children = move_order::GenerateMoveOrderedCaptureOnlyList(
            board);

    for (Move move: children) {
      int score = -Quiesce({board, move}, -beta, -alpha);
      if (score >= beta) {
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }
    return alpha;
  }

  bool IsRepetitionDraw(const Board &board,
                        const std::set<uint64_t> &twofold_repeat_pos) {
    uint64_t hash_value = zobrist::GetHashKey(board);
    return twofold_repeat_pos.find(hash_value) != twofold_repeat_pos.end();
  }
}
