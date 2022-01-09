//
// Created by Yifei Yao on 12/30/21.
//

#ifndef INC_4D_TRANSPOSITION_TABLE_H
#define INC_4D_TRANSPOSITION_TABLE_H

#include <vector>
#include <unordered_map>
#include "board.h"

// Zobrist hash by depth
// Same board at the same depth is a transposition
class TT {
public:
  struct Value {
    enum class Type {
      // kExact for pv nodes, kLowerBound for Fail-high nodes, kUpperBound for All-nodes
      kExact,
      kLowerBound,
      kUpperBound
    };
    Type type;
    int score;
  };

  void AddEntry(const Board &board, Value value, unsigned ply);

  struct Answer {
    bool is_success;
    Value value;
  };

  [[nodiscard]] Answer GetEntry(const Board &board, unsigned ply) const;

private:
  std::vector<std::unordered_map<uint64_t, Value>> table;
};

#endif //INC_4D_TRANSPOSITION_TABLE_H
