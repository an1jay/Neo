#include "bitboard.h"
#include "constants.h"
#include "types.h"

#include <random>

#pragma once

constexpr int MagicBishopMax = 32;
constexpr int MagicRookMax = 256;

constexpr int BishopShifts[] = {
	// clang-format off
	4, 3, 3, 3, 3, 4,
	3, 3, 3, 3, 3, 3,
	3, 3, 5, 5, 3, 3,
	3, 3, 5, 5, 3, 3,
	3, 3, 3, 3, 3, 3,
	4, 3, 3, 3, 3, 4,
	// clang-format on
};

constexpr int RookShifts[] = {
	// clang-format off
	8, 7, 7, 7, 7, 8,
	7, 6, 6, 6, 6, 7,
	7, 6, 6, 6, 6, 7,
	7, 6, 6, 6, 6, 7,
	7, 6, 6, 6, 6, 7,
	8, 7, 7, 7, 7, 8,
	// clang-format on
};

struct Magics
{
      public:
	Magics();
	~Magics();

	BitBoard MagicBishopAttack(Square b, BitBoard occupancy) const;
	BitBoard MagicRookAttack(Square b, BitBoard occupancy) const;
	BitBoard MagicQueenAttack(Square b, BitBoard occupancy) const;

      private:
	BitBoard BishopMagics[numSquaresInBoard];
	BitBoard RookMagics[numSquaresInBoard];

	// TODO: is this correct? Do we want an array of pointers to other arrays?
	BitBoard (*BishopMagicAttacks)[MagicBishopMax];
	BitBoard (*RookMagicAttacks)[MagicRookMax];

	void initBishops();
	void initRooks();
};

BitBoard
genRand(std::mt19937&);

BitBoard
HQBishopAttack(Square b, BitBoard occupancy);

BitBoard
HQRookAttack(Square r, BitBoard occupancy);

BitBoard
HQQueenAttack(Square q, BitBoard occupancy);

void
printBishopMagicOccupancyMask();

void
printRookMagicOccupancyMask();

inline BitBoard
genBishopMagicOccupancyMask(Square s)
{
	return HQBishopAttack(s, NoSquares) & NoEdges;
}

inline BitBoard
genRookMagicOccupancyMask(Square s)
{
	BitBoard relevantMoveMask = HQRookAttack(static_cast<Square>(s), NoSquares);
	BitBoard piece = fromSq(static_cast<Square>(s));
	for (int edge = 0; edge < numEdges; edge++) {
		if ((EdgeBitBoards[edge] & piece) == NoSquares)
			relevantMoveMask &= ~EdgeBitBoards[edge];
	}

	return relevantMoveMask;
}
