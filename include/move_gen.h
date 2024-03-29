#include "magics.h"
#include "types.h"

#pragma once

// Ply encodes as following:
// 00000000000000000 | 0 0 0 | 0 0 0 0 0 0 | 0 0 0 0 0 0
//                   | Prom. |  Origin Sq  |   Dest Sq
//
// Bits 0-5 = Destination Square (0-35)
// Bits 6-11 = Origin Square (0-35)
// Bits 12-14 = Promotion PieceType (Uses PieceType Enum Class)

constexpr int DestSqWidth = 6;
constexpr int OriginSqWidth = DestSqWidth;
constexpr int PromoWidth = 3;

constexpr int DestSqShift = 0;
constexpr int OriginSqShift = DestSqShift + DestSqWidth;
constexpr int PromoShift = OriginSqShift + OriginSqWidth;

constexpr Ply SquareMask = (1ULL << DestSqWidth) - 1;
constexpr Ply PromoMask = (1ULL << PromoWidth) - 1;

inline Square
getDestSquare(Ply p)
{
	return static_cast<Square>((p >> DestSqShift) & SquareMask);
}

inline Square
getOriginSquare(Ply p)
{
	return static_cast<Square>((p >> OriginSqShift) & SquareMask);
}

inline PieceType
getPromoPieceType(Ply p)
{
	Ply promo = ((p >> PromoShift) & PromoMask);
	if (promo)
		return static_cast<PieceType>(promo);
	return PieceType::NB_NONE;
}

inline Ply
encodePly(Square destSq, Square originSq, PieceType promo)
{
	return static_cast<Ply>((static_cast<Ply>(destSq) << DestSqShift) |
				(static_cast<Ply>(originSq) << OriginSqShift) |
				(static_cast<Ply>(promo) << PromoShift));
}

BitBoard
calculateAttackBB(Piece p, Square sq, BitBoard ourOccupancy, BitBoard theirOccupancy, Magics& m);

BitBoard
calculateAttackBB(PieceType p, Square sq, BitBoard ourOccupancy, BitBoard theirOccupancy, Magics& m);