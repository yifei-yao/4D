//
// Created by Yifei Yao on 12/24/21.
//

#include "color.h"
#include <iostream>

namespace color {
  Color FenToColor(char fen_color) {
    switch (fen_color) {
      case 'w':
        return Color::kWhite;
      case 'b':
        return Color::kBlack;
      default: {
        std::cerr << "Invalid FEN color format\n";
        return Color::kNone;
      }
    }
  }

  bool IsOppositeColor(Color lhs, Color rhs) {
    return ((lhs == Color::kWhite && rhs == Color::kBlack) |
            (lhs == Color::kBlack && rhs == Color::kWhite));
  }

  Color operator!(Color color) {
    switch (color) {
      case Color::kWhite:
        return Color::kBlack;
      case Color::kBlack:
        return Color::kWhite;
      case Color::kNone:
        return Color::kNone;
    }
  }

  std::ostream &operator<<(std::ostream &os, Color color) {
    switch (color) {
      case Color::kWhite: {
        os << "Color::kWhite";
        break;
      }
      case Color::kBlack: {
        os << "Color::kBlack";
        break;
      }
      case Color::kNone: {
        os << "Color::kNone";
        break;
      }
    }
    return os;
  }
}
