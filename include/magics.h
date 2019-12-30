#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include <random>

#pragma once

constexpr int MagicBishopMax = 32;
constexpr int MagicRookMax = 256;

constexpr int BishopShifts[] = {
	4, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 3, 3, 3, 3, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 4,
};

constexpr int RookShifts[] = {
	8, 7, 7, 7, 7, 8, 7, 6, 6, 6, 6, 7, 7, 6, 6, 6, 6, 7, 7, 6, 6, 6, 6, 7, 7, 6, 6, 6, 6, 7, 8, 7, 7, 7, 7, 8,
};

struct Magics
{
      public:
	Magics();
	~Magics();

	BitBoard MagicBishopAttack(Square b, BitBoard occupancy) const;
	BitBoard MagicRookAttack(Square sq, BitBoard occupancy) const;
	BitBoard MagicQueenAttack(Square sq, BitBoard occupancy) const;

      private:
	BitBoard BishopMagics[numSquaresInBoard];
	BitBoard RookMagics[numSquaresInBoard];
	BitBoard (*BishopMagicAttacks)[MagicBishopMax];
	BitBoard (*RookMagicAttacks)[MagicRookMax];

	void initBishops();
	void initRooks();
};

BitBoard
genRand(std::mt19937& rng);

BitBoard
HQBishopAttack(Square b, BitBoard occupancy);

BitBoard
HQRookAttack(Square r, BitBoard occupancy);

BitBoard
HQQueenAttack(Square q, BitBoard occupancy);