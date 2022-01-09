//
// Created by Yifei Yao on 12/23/21.
//

#include "board.h"
#include <sstream>

Board::Board() : squares({}), assoc_data(kDefault), en_passant_target(64) {
  DefaultFill();
}

void Board::DefaultFill() {
  FillFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

void Board::FillFen(const std::string &fen_notation_head) {
  unsigned piece_index = 0;
  for (char piece_symbol: fen_notation_head) {
    if (piece_symbol == '/') {
      continue;
    }
    if (isdigit(piece_symbol)) {
      piece_index += piece_symbol - '0';
      continue;
    }
    squares[piece_index++] = Piece(piece_symbol);
  }
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
  for (size_t row = 0; row < 8; ++row) {
    os << 8 - row;
    for (size_t col = 0; col < 8; ++col) {
      os << " " << board.squares[8 * row + col].GetUnicode();
    }
    os << "\n";
  }
  os << "  a b c d e f g h";
  return os;
}

Board::Board(const std::string &fen_notation) : squares({}),
                                                assoc_data(kDefault),
                                                en_passant_target(64) {
  std::stringstream ss;
  ss << fen_notation;
  std::string head, castling, en_passant;
  char color_char;
  if (!(ss >> head >> color_char >> castling >> en_passant)) {
    std::cerr << "Invalid FEN notation format!\n";
    DefaultFill();
    return;
  }
  FillFen(head);
  SetColor(color::FenToColor(color_char));
  SetCastling(castling);
  InitializeEnPassant(en_passant);
}

bool Board::IsFriendlyAt(Position pos) const {
  return squares[pos.GetIndex()].GetColor() == GetColor();
}

Piece::Type Board::GetPieceTypeAt(Position pos) const {
  return squares[pos.GetIndex()].GetType();
}

bool Board::IsSameColor(Position lhs, Position rhs) const {
  return squares[lhs.GetIndex()].GetColor() ==
         squares[rhs.GetIndex()].GetColor();
}

bool Board::IsOppositeColor(Position lhs, Position rhs) const {
  return color::IsOppositeColor(squares[lhs.GetIndex()].GetColor(),
                                squares[rhs.GetIndex()].GetColor());
}

bool Board::IsEmptyAt(Position pos) const {
  return squares[pos.GetIndex()].IsNone();
}

color::Color Board::GetPieceColorAt(Position pos) const {
  return squares[pos.GetIndex()].GetColor();
}

void Board::InitializeEnPassant(const std::string &fen_en_passant) {
  if (fen_en_passant == "-") {
    return;
  }
  if (fen_en_passant.size() != 2) {
    std::cerr << "FEN position format incorrect\n";
    return;
  }
  unsigned col;
  if ('a' <= fen_en_passant[0] && fen_en_passant[0] <= 'h') {
    col = fen_en_passant[0] - 'a';
  } else {
    std::cerr << "FEN position format incorrect\n";
  }
  unsigned row;
  if ('0' <= fen_en_passant[1] && fen_en_passant[1] <= '8') {
    row = 8 - (fen_en_passant[1] - '1');
  } else {
    std::cerr << "FEN position format incorrect\n";
    return;
  }
  en_passant_target = 8 * row + col;
}

bool Board::IsAtEnPassantSquare(Position pos) const {
  return pos.GetIndex() == en_passant_target;
}

bool Board::HasCastling() const {
  switch (assoc_data & kColorMask) {
    case kWhite:
      return assoc_data & kWhiteCastlingMask;
    case kBlack:
      return assoc_data & kBlackCastlingMask;
    default:
      throw std::logic_error(" have color");
  }
}

bool Board::HasKingSideCastling() const {
  switch (assoc_data & kColorMask) {
    case kWhite:
      return assoc_data & kWhiteKingSide;
    case kBlack:
      return assoc_data & kBlackKingSide;
    default:
      throw std::logic_error("Board must have color");
  }
}

bool Board::HasQueenSideCastling() const {
  switch (assoc_data & kColorMask) {
    case kWhite:
      return assoc_data & kWhiteQueenSide;
    case kBlack:
      return assoc_data & kBlackQueenSide;
    default:
      throw std::logic_error("Board must have color");
  }
}

Board::Board(const Board &rhs, Move move) : squares(rhs.squares), assoc_data(
        rhs.assoc_data ^ kColorMask), en_passant_target(64) {
  switch (move.flag) {
    case Move::Flag::kRegular: {
      ApplyDirectMove(move);
      break;
    }
    case Move::Flag::kPawnDoubleMove: {
      ApplyDirectMove(move);
      en_passant_target = ((move.from + move.to) / 2);
      break;
    }
    case Move::Flag::kEnPassantCapture: {
      ApplyDirectMove(move);
      Position capture(Position(move.from).row, Position(move.to).col);
      squares[capture.GetIndex()] = {};
      break;
    }
    case Move::Flag::kPromoteToQueen: {
      squares[move.to] = {Piece::Type::kQueen, rhs.GetColor()};
      squares[move.from] = {};
      break;
    }
    case Move::Flag::kPromoteToRook: {
      squares[move.to] = {Piece::Type::kRook, rhs.GetColor()};
      squares[move.from] = {};
      break;
    }
    case Move::Flag::kPromoteToKnight: {
      squares[move.to] = {Piece::Type::kKnight, rhs.GetColor()};
      squares[move.from] = {};
      break;
    }
    case Move::Flag::kPromoteToBishop: {
      squares[move.to] = {Piece::Type::kBishop, rhs.GetColor()};
      squares[move.from] = {};
      break;
    }
    case Move::Flag::kCastling: {
      DisableCastling(rhs.GetColor());
      ApplyDirectMove(move);
      unsigned mid_index = (move.from + move.to) / 2;
      if (move.to % 8 == 6) {
        squares[mid_index] = squares[move.to + 1];
        squares[move.to + 1] = {};
      } else {
        squares[mid_index] = squares[move.to - 2];
        squares[move.to - 2] = {};
      }
      break;
    }
  }
  static const Piece kBlackRook = Piece{Piece::Type::kRook,
                                        color::Color::kBlack};
  static const Piece kBlackKing = Piece{Piece::Type::kKing,
                                        color::Color::kBlack};
  static const Piece kWhiteRook = Piece{Piece::Type::kRook,
                                        color::Color::kWhite};
  static const Piece kWhiteKing = Piece{Piece::Type::kKing,
                                        color::Color::kWhite};

  if (squares[0] != kBlackRook) {
    assoc_data &= kBlackQueenSideOff;
  }
  if (squares[4] != kBlackKing) {
    assoc_data &= kBlackCastlingOffMask;
  }
  if (squares[7] != kBlackRook) {
    assoc_data &= kBlackKingSideOff;
  }
  if (squares[56] != kWhiteRook) {
    assoc_data &= kWhiteQueenSideOff;
  }
  if (squares[60] != kWhiteKing) {
    assoc_data &= kWhiteCastlingOffMask;
  }
  if (squares[63] != kWhiteRook) {
    assoc_data &= kWhiteKingSideOff;
  }
}

void Board::ApplyDirectMove(Move move) {
  squares[move.to] = squares[move.from];
  squares[move.from] = {};
}

void Board::SetColor(color::Color color) {
  switch (color) {
    case color::Color::kWhite: {
      assoc_data &= kColorOffMask;
      assoc_data |= kWhite;
      break;
    }
    case color::Color::kBlack: {
      assoc_data &= kColorOffMask;
      assoc_data |= kBlack;
      break;
    }
    case color::Color::kNone:
      throw std::logic_error("Cannot assign Color::kNone to Board");
  }
}

void Board::SetCastling(const std::string &fen_castling) {
  assoc_data &= kCastlingOffMask;
  if (fen_castling == "-") {
    return;
  }
  for (char symbol: fen_castling) {
    switch (symbol) {
      case 'K': {
        assoc_data |= kWhiteKingSide;
        break;
      }
      case 'Q': {
        assoc_data |= kWhiteQueenSide;
        break;
      }
      case 'k': {
        assoc_data |= kBlackKingSide;
        break;
      }
      case 'q': {
        assoc_data |= kBlackQueenSide;
        break;
      }
      default: {
        std::cerr << "FEN castling symbol not recognized!\n";
        break;
      }
    }
  }
}

color::Color Board::GetColor() const {
  switch (assoc_data & kColorMask) {
    case kWhite:
      return color::Color::kWhite;
    case kBlack:
      return color::Color::kBlack;
    default:
      throw std::logic_error("Board must have a color");
  }
}

void Board::DisableCastling(color::Color color) {
  switch (color) {
    case color::Color::kWhite: {
      assoc_data &= kWhiteCastlingOffMask;
      break;
    }
    case color::Color::kBlack: {
      assoc_data &= kBlackCastlingOffMask;
      break;
    }
    case color::Color::kNone:
      throw std::logic_error("Cannot disable Color::kNone\n");
  }
}

bool Board::IsEmptyAt(uint_fast8_t index) const {
  return squares[index].IsNone();
}

Piece::Type Board::GetPieceTypeAt(uint_fast8_t pos) const {
  return squares[pos].GetType();
}

color::Color Board::GetPieceColorAt(uint_fast8_t pos) const {
  return squares[pos].GetColor();
}

void Board::Print(color::Color perspective) const {
  if (perspective == color::Color::kWhite) {
    for (size_t row = 0; row < 8; ++row) {
      std::cout << 8 - row;
      for (size_t col = 0; col < 8; ++col) {
        unsigned index = 8 * row + col;
        if (!IsEmptyAt(index)) {
          std::cout << " " << squares[index].GetUnicode();
        } else {
          if ((row + col) % 2 == 0) {
            std::cout << " " << " ";
          } else {
            std::cout << " " << "·";
          }
        }
      }
      std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
  } else {
    for (size_t row = 0; row < 8; ++row) {
      std::cout << 1 + row;
      for (size_t col = 0; col < 8; ++col) {
        unsigned index = 63 - (8 * row + col);
        if (!IsEmptyAt(index)) {
          std::cout << " " << squares[index].GetUnicode();
        } else {
          if ((row + col) % 2 == 0) {
            std::cout << " " << " ";
          } else {
            std::cout << " " << "·";
          }
        }
      }
      std::cout << "\n";
    }
    std::cout << "  h g f e d c b a\n";

  }
}

Piece Board::GetPieceAt(Position pos) const {
  return squares[pos.GetIndex()];
}

Piece Board::GetPieceAt(uint_fast8_t index) const {
  return squares[index];
}

uint_fast8_t Board::GetEnPassantTarget() const {
  return en_passant_target;
}

bool Board::HasActiveEnPassant() const {
  return en_passant_target != 64;
}

bool Board::HasBlackKingSide() const {
  return assoc_data & kBlackKingSide;
}

bool Board::HasBlackQueenSide() const {
  return assoc_data & kBlackQueenSide;
}

bool Board::HasWhiteKingSide() const {
  return assoc_data & kWhiteKingSide;
}

bool Board::HasWhiteQueenSide() const {
  return assoc_data & kWhiteQueenSide;
}
