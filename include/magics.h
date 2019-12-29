#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include <random>

#pragma once

constexpr int MagicBishopMax = 32;
constexpr int MagicRookMax = 256;

constexpr int BishopShifts[] = { 5, 5, 5, 5, 5, 5, 5 };

struct Magics
{
      public:
	Magics();
	~Magics();

	BitBoard AttackForBishop(Square sq, BitBoard occupancy) const;
	BitBoard AttackForRook(Square sq, BitBoard occupancy) const;

	BitBoard HQBishopAttack(Square b, BitBoard occ);
	BitBoard HQRookAttack(Square r, BitBoard occ);

      private:
	BitBoard BishopMagics[numSquaresInBoard];
	BitBoard RookMagics[numSquaresInBoard];
	BitBoard (*BishopMagicAttacks)[MagicBishopMax];
	BitBoard (*RookMagicAttacks)[MagicRookMax];

	void initBishops();
	void initRooks();
};

BitBoard
genSparseRand(std::mt19937& rng);