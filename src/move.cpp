//
// Created by Yifei Yao on 12/24/21.
//

#include "sstream"
#include "move.h"
#include <exception>

Move::Move(Position from, Position to, Move::Flag flag) : from(from.GetIndex()),
                                                          to(to.GetIndex()),
                                                          flag(flag) {}

std::ostream &operator<<(std::ostream &os, Move move) {
  os << Position(move.from) << Position(move.to);
  switch (move.flag) {
    case Move::Flag::kPromoteToRook: {
      os << 'r';
      break;
    }
    case Move::Flag::kPromoteToQueen: {
      os << 'q';
      break;
    }
    case Move::Flag::kPromoteToKnight: {
      os << 'n';
      break;
    }
    case Move::Flag::kPromoteToBishop: {
      os << 'b';
      break;
    }
    default:
      break;
  }
  return os;
}

Move::Move() : from(64), to(64), flag(Flag::kRegular) {}

Move::Move(const std::string &coord_notation) :
        from(Position(coord_notation.substr(0, 2)).GetIndex()),
        to(Position(coord_notation.substr(2, 2)).GetIndex()),
        flag(Flag::kRegular) {
  if (coord_notation.size() == 5) {
    switch (coord_notation[4]) {
      case 'r':
        flag = Flag::kPromoteToRook;
        break;
      case 'q':
        flag = Flag::kPromoteToQueen;
        break;
      case 'b':
        flag = Flag::kPromoteToBishop;
        break;
      case 'n':
        flag = Flag::kPromoteToKnight;
        break;
      default:
        std::cerr << "Wrong notation\n";
        break;
    }
  }
}

std::string Move::GetNotation() const {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}

bool operator==(Move lhs, Move rhs) {
  return (lhs.from == rhs.from) && (lhs.to == rhs.to) && (lhs.flag == rhs.flag);
}

bool operator!=(Move lhs, Move rhs) {
  return !(lhs == rhs);
}
