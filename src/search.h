//
// Created by Yifei Yao on 1/2/22.
//

#ifndef INC_4D_SEARCH_H
#define INC_4D_SEARCH_H

#include "board.h"
#include "move.h"
#include "transposition_table.h"
#include "time_controller.h"
#include <set>

namespace search {
  Move IDRoot(const Board &board, TimeController time_controller,
              const std::set<uint64_t> &unique_pos,
              const std::set<uint64_t> &twofold_repeat_pos);

  std::pair<Move, int> PVSRoot(const Board &board, unsigned depth,
                               const std::vector<Move> &prev_pv,
                               std::vector<Move> &curr_pv,
                               TimeController time_controller,
                               const std::set<uint64_t> &unique_pos,
                               const std::set<uint64_t> &twofold_repeat_pos);

  int PVS(const Board &board, unsigned depth, int alpha, int beta,
          TT &table, unsigned ply, const std::vector<Move> &prev_pv,
          std::vector<Move> &curr_pv, TimeController time_controller);

  int PVSShallow(const Board &board, unsigned depth, int alpha, int beta,
                 TT &table, unsigned ply, const std::vector<Move> &prev_pv,
                 std::vector<Move> &curr_pv, TimeController time_controller,
                 std::set<uint64_t> unique_pos,
                 std::set<uint64_t> twofold_repeat_pos); // same as PVS except consider repetition

  int Quiesce(const Board &board, int alpha, int beta);
}


#endif //INC_4D_SEARCH_H
