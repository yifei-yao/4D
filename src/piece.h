//
// Created by Yifei Yao on 12/23/21.
//

#ifndef INC_4D_PIECE_H
#define INC_4D_PIECE_H

#include "color.h"
#include <iostream>

class Piece {
  friend bool operator!=(Piece lhs, Piece rhs);

  friend bool operator==(Piece lhs, Piece rhs);

public:
  enum class Type : uint_fast8_t {
    kNone, kKing, kQueen, kRook, kBishop, kKnight, kPawn
  };

  [[nodiscard]] color::Color GetColor() const;

  [[nodiscard]] Type GetType() const;

  Piece();

  Piece(Type type, color::Color color);

  [[nodiscard]] std::string GetUnicode() const;

  [[nodiscard]] bool IsNone() const;

  explicit Piece(char fen_symbol);

private:
  color::Color color;
  Type type;
};


#endif //INC_4D_PIECE_H
