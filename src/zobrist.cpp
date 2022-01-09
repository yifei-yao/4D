//
// Created by Yifei Yao on 12/30/21.
//

#include "zobrist.h"
#include <random>

namespace zobrist {
  std::array<std::array<uint64_t, 64>, 13> FillRandomTable() {
    std::array<std::array<uint64_t, 64>, 13> tables = {};
    for (size_t i = 0; i < 64; ++i) {
      for (size_t j = 0; j < 13; ++j) {
        tables[j][i] = GenerateRandom64Int();
      }
    }
    return tables;
  }

  uint64_t GenerateRandom64Int() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint64_t> dist64(0,
                                                   std::numeric_limits<uint64_t>::max());
    return uint64_t(dist64(rng));
  }

  uint64_t GetHashKey(const Board &board) {
    static const std::array<std::array<uint64_t, 64>, 13> random_tables = FillRandomTable();
    static const uint64_t side_to_move = GenerateRandom64Int();
    static const uint64_t kWhiteKingSide = GenerateRandom64Int();
    static const uint64_t kWhiteQueenSide = GenerateRandom64Int();
    static const uint64_t kBlackKingSide = GenerateRandom64Int();
    static const uint64_t kBlackQueenSide = GenerateRandom64Int();
    uint64_t h = 0;
    for (size_t i = 0; i < 64; ++i) {
      if (!board.IsEmptyAt(i)) {
        unsigned color_offset = 0;
        switch (board.GetPieceColorAt(i)) {
          case color::Color::kWhite:
            color_offset = 0;
            break;
          case color::Color::kBlack:
            color_offset = 6;
            break;
          case color::Color::kNone:
            throw std::logic_error("Should not hash empty squares");
        }
        unsigned piece_offset = 0;
        switch (board.GetPieceTypeAt(i)) {
          case Piece::Type::kKing:
            piece_offset = 0;
            break;
          case Piece::Type::kQueen:
            piece_offset = 1;
            break;
          case Piece::Type::kRook:
            piece_offset = 2;
            break;
          case Piece::Type::kBishop:
            piece_offset = 3;
            break;
          case Piece::Type::kKnight:
            piece_offset = 4;
            break;
          case Piece::Type::kPawn:
            piece_offset = 5;
            break;
          case Piece::Type::kNone:
            throw std::logic_error("Should not hash empty squares");
        }
        unsigned index = color_offset + piece_offset;
        uint64_t number = random_tables[index][i];
        h ^= number;
      }
    }
    if (board.HasActiveEnPassant()) {
      h ^= random_tables[12][board.GetEnPassantTarget()];
    }
    if (board.GetColor() == color::Color::kWhite) {
      h ^= side_to_move;
    }
    if (board.HasWhiteKingSide()) {
      h ^= kWhiteKingSide;
    }
    if (board.HasWhiteQueenSide()) {
      h ^= kWhiteQueenSide;
    }
    if (board.HasBlackKingSide()) {
      h ^= kBlackKingSide;
    }
    if (board.HasBlackQueenSide()) {
      h ^= kBlackQueenSide;
    }
    return h;
  }
}
