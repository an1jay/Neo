#include <functional>
#include <iostream>

#include "types.h"

#pragma once

std::string boardPrint(std::function<char(Square)>);

std::ostream&
operator<<(std::ostream&, Square);

Square
sqFromFileRank(const File, const Rank);

File
fileFromSq(const Square);

Rank
rankFromSq(const Square);

Piece
pieceFromPieceTypeColor(const PieceType pt, const Color c);

PieceType
pieceTypeFromPiece(const Piece p);

Color
colorFromPiece(const Piece p);