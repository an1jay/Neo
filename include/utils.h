#include <functional>
#include <iostream>

#include "constants.h"
#include "types.h"

#pragma once

std::string boardPrint(std::function<char(Square)>);

std::ostream&
operator<<(std::ostream&, Square);

constexpr Square
sqFromFileRank(const File f, const Rank r)
{
	if (f == File::NB_NONE || r == Rank::NB_NONE)
		return Square::NB_NONE;
	else
		return static_cast<Square>(static_cast<int>(r) * numSquaresInRankFile + static_cast<int>(f));
}

constexpr File
fileFromSq(const Square s)
{
	return static_cast<File>(static_cast<int>(s) % numSquaresInRankFile);
}

constexpr Rank
rankFromSq(const Square s)
{
	return static_cast<Rank>(static_cast<int>(s) / numSquaresInRankFile);
}

constexpr Piece
pieceFromPieceTypeColor(const PieceType pt, const Color c)
{
	if (pt == PieceType::NB_NONE || c == Color::NB_NONE)
		return Piece::NB_NONE;
	return static_cast<Piece>(static_cast<int>(pt) * (static_cast<int>(c) + 1));
}

constexpr PieceType
pieceTypeFromPiece(const Piece p)
{
	if (p == Piece::NB_NONE)
		return PieceType::NB_NONE;
	return static_cast<PieceType>(static_cast<int>(p) % static_cast<int>(Piece::B_Pawn));
}

constexpr Color
colorFromPiece(const Piece p)
{
	if (p == Piece::NB_NONE)
		return Color::NB_NONE;
	if (static_cast<int>(p) <= static_cast<int>(Piece::W_King))
		return Color::White;
	return Color::Black;
}

constexpr inline Color
otherColor(Color c)
{
	if (c == Color::Black)
		return Color::White;
	else if (c == Color::White)
		return Color::Black;
	return Color::NB_NONE;
}