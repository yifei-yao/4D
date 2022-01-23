//
// Created by Yifei Yao on 12/24/21.
//

#include <stdexcept>
#include <array>
#include "move_gen.h"
#include "piece.h"
#include "position.h"

namespace move_gen {
  std::vector<Move> GenerateAllMoves(const Board &board) {
    Position king_pos = {-1, -1};
    std::vector<Position> friend_list;
    for (Position pos{0}; pos.IsInRange(); ++pos) {
      if (board.IsFriendlyAt(pos)) {
        if (board.GetPieceTypeAt(pos) == Piece::Type::kKing) {
          king_pos = pos;
        }
        friend_list.push_back(pos);
      }
    }
    std::vector<Move> moves;
    for (Position pos: friend_list) {
      switch (board.GetPieceTypeAt(pos)) {
        case Piece::Type::kKing: {
          GenerateKingMoves(board, pos, moves);
          break;
        }
        case Piece::Type::kQueen: {
          GenerateQueenMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kRook: {
          GenerateRookMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kBishop: {
          GenerateBishopMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kKnight: {
          GenerateKnightMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kPawn: {
          GeneratePawnMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kNone: {
          throw std::logic_error("Type::kNone for same color piece");
        }
      }
    }
    GenerateCastlingMoves(board, moves, king_pos);
    return moves;
  }

  void GenerateKingMoves(const Board &board, Position from,
                         std::vector<Move> &moves) {
    for (Position offset: direction::kAllDirections) {
      Position target = offset + from;
      if (target.IsInRange()) {
        if (!board.IsSameColor(from, target)) {
          ValidatePush(board, moves, {from, target}, target);
        }
      }
    }
  }

  void GenerateKingCaptureMoves(const Board &board, Position from,
                                std::vector<Move> &moves) {
    for (Position offset: direction::kAllDirections) {
      Position target = offset + from;
      if (target.IsInRange()) {
        if (board.IsOppositeColor(from, target)) {
          ValidatePush(board, moves, {from, target}, target);
        }
      }
    }
  }

  void GenerateQueenMoves(const Board &board, Position from,
                          std::vector<Move> &moves, Position king_pos) {
    for (Position offset: direction::kAllDirections) {
      for (Position target =
              from + offset; target.IsInRange(); target += offset) {
        if (board.IsEmptyAt(target)) {
          ValidatePush(board, moves, {from, target}, king_pos);
        } else {
          if (board.IsSameColor(from, target)) {
            break;
          } else {
            ValidatePush(board, moves, {from, target}, king_pos);
            break;
          }
        }
      }
    }
  }

  void GenerateRookMoves(const Board &board, Position from,
                         std::vector<Move> &moves, Position king_pos) {
    static const std::array<Position, 4> directions = {direction::kNorth,
                                                       direction::kSouth,
                                                       direction::kWest,
                                                       direction::kEast};
    for (Position offset: directions) {
      for (Position target =
              from + offset; target.IsInRange(); target += offset) {
        if (board.IsEmptyAt(target)) {
          ValidatePush(board, moves, {from, target}, king_pos);
        } else {
          if (board.IsSameColor(from, target)) {
            break;
          } else {
            ValidatePush(board, moves, {from, target}, king_pos);
            break;
          }
        }
      }
    }
  }

  void GenerateRookCaptureMoves(const Board &board, Position from,
                                std::vector<Move> &moves, Position king_pos) {
    static const std::array<Position, 4> directions = {direction::kNorth,
                                                       direction::kSouth,
                                                       direction::kWest,
                                                       direction::kEast};
    for (Position offset: directions) {
      for (Position target =
              from + offset; target.IsInRange(); target += offset) {
        if (board.IsEmptyAt(target)) {
          continue;
        } else {
          if (board.IsSameColor(from, target)) {
            break;
          } else {
            ValidatePush(board, moves, {from, target}, king_pos);
            break;
          }
        }
      }
    }
  }

  void GenerateBishopMoves(const Board &board, Position from,
                           std::vector<Move> &moves, Position king_pos) {
    static const std::array<Position, 4> directions = {direction::kNorthWest,
                                                       direction::kNorthEast,
                                                       direction::kSouthWest,
                                                       direction::kSouthEast};
    for (Position offset: directions) {
      for (Position target =
              from + offset; target.IsInRange(); target += offset) {
        if (board.IsEmptyAt(target)) {
          ValidatePush(board, moves, {from, target}, king_pos);
        } else {
          if (board.IsSameColor(from, target)) {
            break;
          } else {
            ValidatePush(board, moves, {from, target}, king_pos);
            break;
          }
        }
      }
    }
  }

  void GenerateKnightMoves(const Board &board, Position from,
                           std::vector<Move> &moves, Position king_pos) {
    static const std::array<Position, 8> directions = {{{-2, -1},
                                                        {-2, 1},
                                                        {-1, -2},
                                                        {-1, 2},
                                                        {1, -2},
                                                        {1, 2},
                                                        {2, -1},
                                                        {2, 1}}};
    for (Position offset: directions) {
      Position target = offset + from;
      if (target.IsInRange()) {
        if (!board.IsSameColor(from, target)) {
          ValidatePush(board, moves, {from, target}, king_pos);
        }
      }
    }
  }

  void GeneratePawnMoves(const Board &board, Position from,
                         std::vector<Move> &moves, Position king_pos) {
    Position forward;
    unsigned eighth_rank;
    unsigned initial_rank;
    switch (board.GetPieceColorAt(from)) {
      case color::Color::kWhite: {
        forward = direction::kNorth;
        eighth_rank = 0;
        initial_rank = 6;
        break;
      }
      case color::Color::kBlack: {
        forward = direction::kSouth;
        eighth_rank = 7;
        initial_rank = 1;
        break;
      }
      case color::Color::kNone: {
        throw std::logic_error("Pawn Color::KNone\n");
      }
    }
    Position forward1 = from + forward;
    if (forward1.IsInRange() && board.IsEmptyAt(forward1)) {
      if (forward1.row == eighth_rank) {
        ValidatePush(board, moves,
                     {from, forward1, Move::Flag::kPromoteToQueen}, king_pos);
        ValidatePush(board, moves, {from, forward1,
                                    Move::Flag::kPromoteToRook}, king_pos);
        ValidatePush(board, moves,
                     {from, forward1, Move::Flag::kPromoteToBishop}, king_pos);
        ValidatePush(board, moves, {from, forward1,
                                    Move::Flag::kPromoteToKnight}, king_pos);
      } else {
        ValidatePush(board, moves, {from, forward1}, king_pos);
        if (from.row == initial_rank) {
          Position forward2 = forward1 + forward;
          if (board.IsEmptyAt(forward2)) {
            ValidatePush(board, moves, {from, forward2,
                                        Move::Flag::kPawnDoubleMove}, king_pos);
          }
        }
      }
    }
    Position forward_west = forward1 + direction::kWest;
    if (forward_west.IsInRange()) {
      if (board.IsOppositeColor(from, forward_west)) {
        if (forward_west.row == eighth_rank) {
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToQueen},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToBishop},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToKnight},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToRook},
                       king_pos);
        } else {
          ValidatePush(board, moves, {from, forward_west}, king_pos);
        }
      } else if (board.IsAtEnPassantSquare(forward_west)) {
        ValidatePush(board, moves, {from, forward_west,
                                    Move::Flag::kEnPassantCapture}, king_pos);
      }
    }
    Position forward_east = forward1 + direction::kEast;
    if (forward_east.IsInRange()) {
      if (board.IsOppositeColor(from, forward_east)) {
        if (forward_east.row == eighth_rank) {
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToQueen},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToBishop},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToKnight},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToRook},
                       king_pos);
        } else {
          ValidatePush(board, moves, {from, forward_east}, king_pos);
        }
      } else if (board.IsAtEnPassantSquare(forward_east)) {
        ValidatePush(board, moves,
                     {from, forward_east, Move::Flag::kEnPassantCapture},
                     king_pos);
      }
    }
  }

  void GenerateCastlingMoves(const Board &board, std::vector<Move> &moves,
                             Position king_pos) {
    if (board.HasCastling()) {
      if (IsInCheck(board, king_pos)) {
        return;
      }
      if (board.HasKingSideCastling()) {
        Position pos1 = king_pos + direction::kEast;
        Position target = pos1 + direction::kEast;
        if (board.IsEmptyAt(pos1) && board.IsEmptyAt(target)) {
          Board pos1_board(board, Move{king_pos, pos1});
          if (!IsInCheck({board, Move{king_pos, pos1}}, pos1) &&
              !IsInCheck({board, Move{king_pos, target}}, target)) {
            moves.emplace_back(king_pos, target, Move::Flag::kCastling);
          }
        }
      }
      if (board.HasQueenSideCastling()) {
        Position pos1 = king_pos + direction::kWest;
        Position target = pos1 + direction::kWest;
        if (board.IsEmptyAt(pos1) && board.IsEmptyAt(target) &&
            board.IsEmptyAt(target + direction::kWest)) {
          Board pos1_board(board, Move{king_pos, pos1});
          if (!IsInCheck({board, Move{king_pos, pos1}}, pos1) &&
              !IsInCheck({board, Move{king_pos, target}}, target)) {
            moves.emplace_back(king_pos, target, Move::Flag::kCastling);
          }
        }
      }
    }
  }

  bool IsInCheck(const Board &board, Position king_pos) {
    color::Color mover_color = board.GetPieceColorAt(king_pos);
    static const std::array<Position, 4> kBishopDirections = {
            direction::kNorthWest, direction::kNorthEast,
            direction::kSouthWest,
            direction::kSouthEast};
    for (Position offset: kBishopDirections) {
      for (Position target =
              king_pos + offset; target.IsInRange(); target += offset) {
        Piece piece = board.GetPieceAt(target);
        color::Color color = piece.GetColor();
        if (color == color::Color::kNone) {
          continue;
        } else if (color == mover_color) {
          break;
        } else {
          Piece::Type type = piece.GetType();
          if ((type == Piece::Type::kBishop) || (type == Piece::Type::kQueen)) {
            return true;
          }
          break;
        }
      }
    }
    static const std::array<Position, 4> kRookDirections = {direction::kNorth,
                                                            direction::kSouth,
                                                            direction::kWest,
                                                            direction::kEast};
    for (Position offset: kRookDirections) {
      for (Position target =
              king_pos + offset; target.IsInRange(); target += offset) {
        Piece piece = board.GetPieceAt(target);
        color::Color color = piece.GetColor();
        if (color == color::Color::kNone) {
          continue;
        } else if (color == mover_color) {
          break;
        } else {
          Piece::Type type = piece.GetType();
          if ((type == Piece::Type::kRook) || (type == Piece::Type::kQueen)) {
            return true;
          }
          break;
        }
      }
    }
    static const std::array<Position, 8> kKnightDirections = {{{-2, -1},
                                                               {-2, 1},
                                                               {-1, -2},
                                                               {-1, 2},
                                                               {1, -2},
                                                               {1, 2},
                                                               {2, -1},
                                                               {2, 1}}};
    for (Position offset: kKnightDirections) {
      Position target = king_pos + offset;
      if (target.IsInRange()) {
        Piece piece = board.GetPieceAt(target);
        if ((piece.GetType() == Piece::Type::kKnight) &&
            (piece.GetColor() != mover_color)) {
          return true;
        }
      }
    }
    switch (mover_color) {
      case color::Color::kWhite: {
        static const std::array<Position, 2> kPawnAttack = {
                direction::kNorthWest, direction::kNorthEast};
        for (Position offset: kPawnAttack) {
          Position target = king_pos + offset;
          if (target.IsInRange()) {
            Piece piece = board.GetPieceAt(target);
            if (piece.GetColor() == color::Color::kBlack &&
                piece.GetType() == Piece::Type::kPawn) {
              return true;
            }
          }
        }
        break;
      }
      case color::Color::kBlack: {
        static const std::array<Position, 2> kPawnAttack = {
                direction::kSouthWest, direction::kSouthEast};
        for (Position offset: kPawnAttack) {
          Position target = king_pos + offset;
          if (target.IsInRange()) {
            Piece piece = board.GetPieceAt(target);
            if (piece.GetColor() == color::Color::kWhite &&
                piece.GetType() == Piece::Type::kPawn) {
              return true;
            }
          }
        }
        break;
      }
      case color::Color::kNone: {
        throw std::logic_error("King type has Color::kNone\n");
      }
    }
    for (Position offset: direction::kAllDirections) {
      Position target = king_pos + offset;
      if (target.IsInRange() &&
          board.GetPieceTypeAt(target) == Piece::Type::kKing) {
        return true;
      }
    }
    return false;
  }

  void ValidatePush(const Board &board, std::vector<Move> &moves, Move move,
                    Position king_pos) {
    Board new_board(board, move);
    if (!IsInCheck(new_board, king_pos)) {
      moves.push_back(move);
    }
  }

  bool IsHasNoValidMove(const Board &board) {
    return GenerateAllMoves(board).empty();
  }

  std::vector<Move> GenerateCaptureOnly(const Board &board) {
    Position king_pos = {-1, -1};
    std::vector<Position> friend_list;
    for (Position pos{0}; pos.IsInRange(); ++pos) {
      if (board.IsFriendlyAt(pos)) {
        if (board.GetPieceTypeAt(pos) == Piece::Type::kKing) {
          king_pos = pos;
        }
        friend_list.push_back(pos);
      }
    }
    std::vector<Move> moves;
    for (Position pos: friend_list) {
      switch (board.GetPieceTypeAt(pos)) {
        case Piece::Type::kKing: {
          GenerateKingCaptureMoves(board, pos, moves);
          break;
        }
        case Piece::Type::kQueen: {
          GenerateQueenCaptureMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kRook: {
          GenerateRookCaptureMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kBishop: {
          GenerateBishopCaptureMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kKnight: {
          GenerateKnightCaptureMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kPawn: {
          GeneratePawnCaptureMoves(board, pos, moves, king_pos);
          break;
        }
        case Piece::Type::kNone: {
          throw std::logic_error("Type::kNone for same color piece");
        }
      }
    }
    return moves;
  }

  void GenerateQueenCaptureMoves(const Board &board, Position from,
                                 std::vector<Move> &moves, Position king_pos) {
    for (Position offset: direction::kAllDirections) {
      for (Position target =
              from + offset; target.IsInRange(); target += offset) {
        if (board.IsEmptyAt(target)) {
          continue;
        } else {
          if (board.IsSameColor(from, target)) {
            break;
          } else {
            ValidatePush(board, moves, {from, target}, king_pos);
            break;
          }
        }
      }
    }

  }

  void GenerateBishopCaptureMoves(const Board &board, Position from,
                                  std::vector<Move> &moves, Position king_pos) {
    static const std::array<Position, 4> directions = {direction::kNorthWest,
                                                       direction::kNorthEast,
                                                       direction::kSouthWest,
                                                       direction::kSouthEast};
    for (Position offset: directions) {
      for (Position target =
              from + offset; target.IsInRange(); target += offset) {
        if (board.IsEmptyAt(target)) {
          continue;
        } else {
          if (board.IsSameColor(from, target)) {
            break;
          } else {
            ValidatePush(board, moves, {from, target}, king_pos);
            break;
          }
        }
      }
    }
  }

  void GenerateKnightCaptureMoves(const Board &board, Position from,
                                  std::vector<Move> &moves, Position king_pos) {
    static const std::array<Position, 8> directions = {{{-2, -1},
                                                        {-2, 1},
                                                        {-1, -2},
                                                        {-1, 2},
                                                        {1, -2},
                                                        {1, 2},
                                                        {2, -1},
                                                        {2, 1}}};
    for (Position offset: directions) {
      Position target = offset + from;
      if (target.IsInRange()) {
        if (board.IsOppositeColor(from, target)) {
          ValidatePush(board, moves, {from, target}, king_pos);
        }
      }
    }

  }

  void GeneratePawnCaptureMoves(const Board &board, Position from,
                                std::vector<Move> &moves, Position king_pos) {
    Position forward;
    unsigned eighth_rank;
    unsigned initial_rank;
    switch (board.GetPieceColorAt(from)) {
      case color::Color::kWhite: {
        forward = direction::kNorth;
        eighth_rank = 0;
        initial_rank = 6;
        break;
      }
      case color::Color::kBlack: {
        forward = direction::kSouth;
        eighth_rank = 7;
        initial_rank = 1;
        break;
      }
      case color::Color::kNone: {
        throw std::logic_error("Pawn Color::KNone\n");
      }
    }
    Position forward1 = from + forward;
    Position forward_west = forward1 + direction::kWest;
    if (forward_west.IsInRange()) {
      if (board.IsOppositeColor(from, forward_west)) {
        if (forward_west.row == eighth_rank) {
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToQueen},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToBishop},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToKnight},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_west, Move::Flag::kPromoteToRook},
                       king_pos);
        } else {
          ValidatePush(board, moves, {from, forward_west}, king_pos);
        }
      } else if (board.IsAtEnPassantSquare(forward_west)) {
        ValidatePush(board, moves, {from, forward_west,
                                    Move::Flag::kEnPassantCapture}, king_pos);
      }
    }
    Position forward_east = forward1 + direction::kEast;
    if (forward_east.IsInRange()) {
      if (board.IsOppositeColor(from, forward_east)) {
        if (forward_east.row == eighth_rank) {
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToQueen},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToBishop},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToKnight},
                       king_pos);
          ValidatePush(board, moves,
                       {from, forward_east, Move::Flag::kPromoteToRook},
                       king_pos);
        } else {
          ValidatePush(board, moves, {from, forward_east}, king_pos);
        }
      } else if (board.IsAtEnPassantSquare(forward_east)) {
        ValidatePush(board, moves,
                     {from, forward_east, Move::Flag::kEnPassantCapture},
                     king_pos);
      }
    }
  }

  Position FindKingPos(const Board &board) {
    Position king_pos = {-1, -1};
    for (Position pos{0}; pos.IsInRange(); ++pos) {
      if (board.IsFriendlyAt(pos)) {
        if (board.GetPieceTypeAt(pos) == Piece::Type::kKing) {
          king_pos = pos;
          break;
        }
      }
    }
    return king_pos;
  }
}
