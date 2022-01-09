//
// Created by Yifei Yao on 12/30/21.
//

#ifndef INC_4D_MOVE_ORDER_H
#define INC_4D_MOVE_ORDER_H

#include "move.h"
#include "board.h"
#include "unordered_map"
#include "zobrist.h"
#include <list>

namespace move_order {
  int GetStaticMoveValue(Move move, const Board &pre_move_board);

  std::list<Move>
  GenerateMoveOrderedChildrenList(const Board &board, unsigned ply,
                                  const std::vector<Move> &prev_pvs);

  std::vector<Move> GenerateMoveOrderedCaptureOnlyList(const Board &board);
}


#endif //INC_4D_MOVE_ORDER_H
