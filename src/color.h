//
// Created by Yifei Yao on 12/23/21.
//

#ifndef INC_4D_COLOR_H
#define INC_4D_COLOR_H

#include <cstdint>
#include <ostream>

namespace color {
  enum class Color : uint_fast8_t {
    kNone, kWhite, kBlack
  };

  Color operator!(Color color);

  Color FenToColor(char fen_color);

  bool IsOppositeColor(Color lhs, Color rhs);

  std::ostream &operator<<(std::ostream &os, Color color);
}
#endif //INC_4D_COLOR_H
