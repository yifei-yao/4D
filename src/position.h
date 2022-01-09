//
// Created by Yifei Yao on 12/24/21.
//

#ifndef INC_4D_POSITION_H
#define INC_4D_POSITION_H

#include <iostream>
#include <string>

struct Position {
  friend std::ostream &operator<<(std::ostream &os, Position);

  Position();

  explicit Position(const std::string &notation);

  void operator+=(const Position &rhs);

  explicit Position(unsigned index);

  Position(int row, int col);

  void operator++();

  [[nodiscard]] bool IsInRange() const;

  [[nodiscard]] unsigned GetIndex() const;

  int row;
  int col;
};

Position operator+(Position lhs, Position rhs);


#endif //INC_4D_POSITION_H
