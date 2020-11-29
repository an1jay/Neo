#include <functional>
#include <iostream>

#include "types.h"

#pragma once

std::string
boardPrint(std::function<char(Square)>);

std::ostream&
operator<<(std::ostream&, Square);

constexpr Square
sqFromFileRank(const File, const Rank);

constexpr File
fileFromSq(const Square);

constexpr Rank
rankFromSq(const Square);

constexpr Piece
pieceFromPieceTypeColor(const PieceType pt, const Color c);

constexpr PieceType
pieceTypeFromPiece(const Piece p);

constexpr Color
colorFromPiece(const Piece p);

constexpr inline Color
otherColor(Color c);