//
// Created by Yifei Yao on 12/24/21.
//

#ifndef INC_4D_MOVE_H
#define INC_4D_MOVE_H

#include "position.h"
#include "iostream"

struct Move {
  friend bool operator==(Move lhs, Move rhs);

  friend bool operator!=(Move lhs, Move rhs);

  friend std::ostream &operator<<(std::ostream &os, Move move);

  [[nodiscard]] std::string GetNotation() const;

  Move();

  explicit Move(const std::string &coord_notation);

  unsigned from;
  unsigned to;

  enum class Flag {
    kRegular,
    kCastling,
    kEnPassantCapture,
    kPawnDoubleMove,
    kPromoteToQueen,
    kPromoteToRook,
    kPromoteToBishop,
    kPromoteToKnight
  };
  Flag flag;

  Move(Position from, Position to, Flag flag = Flag::kRegular);
};


#endif //INC_4D_MOVE_H
