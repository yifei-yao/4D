//
// Created by Yifei Yao on 12/30/21.
//

#ifndef INC_4D_ZOBRIST_H
#define INC_4D_ZOBRIST_H

#include "board.h"
#include <array>
#include <vector>
#include <unordered_map>

namespace zobrist {
  std::array<std::array<uint64_t, 64>, 13> FillRandomTable();

  uint64_t GenerateRandom64Int();

  uint64_t GetHashKey(const Board &board);
}

#endif //INC_4D_ZOBRIST_H
