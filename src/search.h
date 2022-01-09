//
// Created by Yifei Yao on 1/2/22.
//

#ifndef INC_4D_SEARCH_H
#define INC_4D_SEARCH_H

#include "board.h"
#include "move.h"
#include "transposition_table.h"
#include "time_controller.h"

namespace search {
  Move IDRoot(const Board &board, TimeController time_controller);

  std::pair<Move, int> PVSRoot(const Board &board, unsigned depth,
               const std::vector<Move> &prev_pv,
               std::vector<Move> &curr_pv, TimeController time_controller);

  int PVS(const Board &board, unsigned depth, int alpha, int beta,
          TT &table, unsigned ply, const std::vector<Move> &prev_pv,
          std::vector<Move> &curr_pv, TimeController time_controller);

  int Quiesce(const Board &board, int alpha, int beta);
}


#endif //INC_4D_SEARCH_H
