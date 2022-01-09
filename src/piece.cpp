//
// Created by Yifei Yao on 12/24/21.
//
#include <string>
#include <iostream>
#include "piece.h"

Piece::Piece() : type(Type::kNone), color(color::Color::kNone) {}

Piece::Piece(char fen_symbol) : type(Type::kNone), color(color::Color::kNone) {
  if (isupper(fen_symbol)) {
    color = color::Color::kWhite;
    fen_symbol = char(tolower(fen_symbol));
  } else if (islower(fen_symbol)) {
    color = color::Color::kBlack;
  } else {
    std::cerr << "FEN piece symbol not recognized\n";
    return;
  }
  switch (fen_symbol) {
    case 'k': {
      type = Type::kKing;
      break;
    }
    case 'q': {
      type = Type::kQueen;
      break;
    }
    case 'r': {
      type = Type::kRook;
      break;
    }
    case 'b': {
      type = Type::kBishop;
      break;
    }
    case 'n': {
      type = Type::kKnight;
      break;
    }
    case 'p': {
      type = Type::kPawn;
      break;
    }
    default: {
      std::cerr << "FEN piece symbol not recognized\n";
      break;
    }
  }
}

std::string Piece::GetUnicode() const {
  switch (GetColor()) {
    case color::Color::kWhite: {
      switch (GetType()) {
        case Type::kKing:
          return "♔";
        case Type::kQueen:
          return "♕";
        case Type::kRook:
          return "♖";
        case Type::kBishop:
          return "♗";
        case Type::kKnight:
          return "♘";
        case Type::kPawn:
          return "♙";
        case Type::kNone:
          return "·";
      }
    }
    case color::Color::kBlack: {
      switch (GetType()) {
        case Type::kKing:
          return "♚";
        case Type::kQueen:
          return "♛";
        case Type::kRook:
          return "♜";
        case Type::kBishop:
          return "♝";
        case Type::kKnight:
          return "♞";
        case Type::kPawn:
          return "♟︎";
        case Type::kNone:
          return "·";
      }
    }
    case color::Color::kNone:
      return "·";
  }
}

bool Piece::IsNone() const {
  return ((type == Type::kNone) || (color == color::Color::kNone));
}

Piece::Piece(Piece::Type type, color::Color color) : type(type), color(color) {}

color::Color Piece::GetColor() const {
  return color;
}

Piece::Type Piece::GetType() const {
  return type;
}

bool operator!=(Piece lhs, Piece rhs) {
  return (lhs.type != rhs.type) || (lhs.color != rhs.color);
}

bool operator==(Piece lhs, Piece rhs) {
  return (lhs.color == rhs.color) && (lhs.type == rhs.type);
}
