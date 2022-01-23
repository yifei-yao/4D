//
// Created by Yifei Yao on 12/23/21.
//

#ifndef INC_4D_BOARD_H
#define INC_4D_BOARD_H

#include <array>
#include <string>
#include <iostream>
#include "move.h"
#include "piece.h"
#include "color.h"
#include "position.h"

class Board {
  friend std::ostream &operator<<(std::ostream &os, const Board &board);

public:
  Board();

  void Print(color::Color perspective) const;

  explicit Board(const std::string &main_fen_notation);

  [[nodiscard]] Piece GetPieceAt(Position pos) const;

  [[nodiscard]] Piece GetPieceAt(uint_fast8_t index) const;

  [[nodiscard]] Piece::Type GetPieceTypeAt(Position pos) const;

  [[nodiscard]] Piece::Type GetPieceTypeAt(uint_fast8_t pos) const;

  [[nodiscard]] color::Color GetPieceColorAt(Position pos) const;

  [[nodiscard]] color::Color GetPieceColorAt(uint_fast8_t pos) const;

  Board(const Board &rhs, Move move);

  [[nodiscard]] bool HasCastling() const;

  [[nodiscard]] bool HasKingSideCastling() const;

  [[nodiscard]] bool HasQueenSideCastling() const;

  [[nodiscard]] bool IsFriendlyAt(Position pos) const;

  [[nodiscard]] bool IsSameColor(Position lhs, Position rhs) const;

  [[nodiscard]] bool IsOppositeColor(Position lhs, Position rhs) const;

  [[nodiscard]] bool IsEmptyAt(Position pos) const;

  [[nodiscard]] bool IsEmptyAt(uint_fast8_t index) const;

  [[nodiscard]] bool IsAtEnPassantSquare(Position pos) const;

  [[nodiscard]] uint_fast8_t GetEnPassantTarget() const;

  [[nodiscard]] bool HasActiveEnPassant() const;

  void SetColor(color::Color color);

  void SetCastling(const std::string &castling);

  [[nodiscard]] color::Color GetColor() const;

  void DisableCastling(color::Color color);

  [[nodiscard]] bool HasBlackKingSide() const;

  [[nodiscard]] bool HasBlackQueenSide() const;

  [[nodiscard]] bool HasWhiteKingSide() const;

  [[nodiscard]] bool HasWhiteQueenSide() const;

private:
  void DefaultFill();

  void FillFen(const std::string &fen_notation_head);

  void InitializeEnPassant(const std::string &fen_en_passant);

  void ApplyDirectMove(Move move);

  std::array<Piece, 64> squares;

  uint_fast8_t assoc_data;  // store active color and castling information

  uint_fast8_t en_passant_target; // position behind a pawn after its two-square move; defaults at 64

  static constexpr uint_fast8_t kWhiteKingSide = 0b0001;
  static constexpr uint_fast8_t kWhiteKingSideOff = ~kWhiteKingSide;
  static constexpr uint_fast8_t kWhiteQueenSide = 0b0010;
  static constexpr uint_fast8_t kWhiteQueenSideOff = ~kWhiteQueenSide;
  static constexpr uint_fast8_t kBlackKingSide = 0b0100;
  static constexpr uint_fast8_t kBlackKingSideOff = ~kBlackKingSide;
  static constexpr uint_fast8_t kBlackQueenSide = 0b1000;
  static constexpr uint_fast8_t kBlackQueenSideOff = ~kBlackQueenSide;
  static constexpr uint_fast8_t kCastlingMask = 0b1111;
  static constexpr uint_fast8_t kCastlingOffMask = 0b11110000;
  static constexpr uint_fast8_t kWhiteCastlingMask = 0b0011;
  static constexpr uint_fast8_t kWhiteCastlingOffMask = ~kWhiteCastlingMask;
  static constexpr uint_fast8_t kBlackCastlingMask = 0b1100;
  static constexpr uint_fast8_t kBlackCastlingOffMask = ~kBlackCastlingMask;

  static constexpr uint_fast8_t kWhite = 0b10000;
  static constexpr uint_fast8_t kBlack = 0b100000;
  static constexpr uint_fast8_t kColorMask = 0b110000;
  static constexpr uint_fast8_t kColorOffMask = ~kColorMask;

  static constexpr uint_fast8_t kDefault = kWhite | kCastlingMask;
};

#endif //INC_4D_BOARD_H
