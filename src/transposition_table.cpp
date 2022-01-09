//
// Created by Yifei Yao on 12/30/21.
//

#include "transposition_table.h"
#include "zobrist.h"

void TT::AddEntry(const Board &board, TT::Value value, unsigned int ply) {
  while (table.size() < ply) {
    table.emplace_back();
  }
  table[ply - 1].emplace(zobrist::GetHashKey(board), value);
}

TT::Answer TT::GetEntry(const Board &board, unsigned int ply) const {
  if (table.size() < ply) {
    return {false, {}};
  }
  auto found = table[ply - 1].find(zobrist::GetHashKey(board));
  if (found == table[ply - 1].end()) {
    return {false, {}};
  } else {
    return {true, found->second};
  }
}
