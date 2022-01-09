//
// Created by Yifei Yao on 12/24/21.
//

#include "position.h"

Position::Position(unsigned int index) : row(int(index / 8)),
                                         col(int(index % 8)) {}

Position::Position(int row, int col) : row(row), col(col) {}

bool Position::IsInRange() const {
  return (0 <= row) && (row < 8) && (0 <= col) && (col < 8);
}

unsigned Position::GetIndex() const {
  return 8 * row + col;
}

void Position::operator+=(const Position &rhs) {
  row += rhs.row;
  col += rhs.col;
}

Position::Position() : row(-1), col(-1) {}

void Position::operator++() {
  if (col == 7) {
    col = 0;
    ++row;
  } else {
    ++col;
  }
}

std::ostream &operator<<(std::ostream &os, Position pos) {
  os << char(char(pos.col) + 'a') << 8 - pos.row;
  return os;
}

Position::Position(const std::string &notation) {
  if (notation.size() != 2) {
    throw std::range_error("Incorrect notation format");
  }
  col = notation[0] - 'a';
  row = 8 - (notation[1] - '0');
}

Position operator+(Position lhs, Position rhs) {
  return {lhs.row + rhs.row, lhs.col + rhs.col};
}
