#include "magics.h"
#include "bitboard.h"
#include "constants.h"
#include "utils.h"
#include <random>

Magics::Magics()
{
	BishopMagicAttacks = new BitBoard[numSquaresInBoard][MagicBishopMax];
	RookMagicAttacks = new BitBoard[numSquaresInBoard][MagicRookMax];
	for (int sq = 0; sq < numSquaresInBoard; ++sq) {
		// set bishop magics to full board
		for (int b = 0; b < MagicBishopMax; b++) {
			BishopMagicAttacks[sq][b] = AllSquares;
		}
		// set rook magics to full board
		for (int r = 0; r < MagicRookMax; r++) {
			RookMagicAttacks[sq][r] = AllSquares;
		}
	}
	initBishops();
	initRooks();
}

Magics::~Magics()
{
	delete[] BishopMagicAttacks;
	delete[] RookMagicAttacks;
}

void
Magics::initBishops()
{
	// 1. For each square find a candidate magic
	// 2. For each possible occupancy, store attack vector in correct index, checking there is no collision
	// 3. If no collisions, we have valid magic
	std::mt19937 rng = std::mt19937();
	BitBoard candidateMagic;
	BitBoard relevantMoveMask;
	BitBoard occupancy;
	BitBoard moves;
	BitBoard index;
	bool validMagic;
	int triedCount;
	int totalTried = 0;
	for (int sq = 0; sq < numSquaresInBoard; ++sq) {
		triedCount = 0;
		// only care about middle 6x6 occupancies
		relevantMoveMask = HQBishopAttack(static_cast<Square>(sq), NoSquares) & NoEdges;
		// assume valid until proven otherwise
		validMagic = true;
		// inside this loop, occupancy is equal to every possible relevant occupancy
		do {
			// part of Carry Rippler (see https://www.chessprogramming.org/Traversing_Subsets_of_a_Set)
			occupancy = 0ULL;
			validMagic = true; // assume valid till proven otherwise
			// find a random magic (see https://www.chessprogramming.org/Looking_for_Magics)
			candidateMagic = genRand(rng) & genRand(rng) & genRand(rng);
			++triedCount;
			// set bishop magics to full board
			for (int b = 0; b < MagicBishopMax; b++) {
				BishopMagicAttacks[sq][b] = AllSquares;
			}
			do {
				moves = HQBishopAttack(static_cast<Square>(sq), occupancy);
				index = (occupancy * candidateMagic) >> (64 - BishopShifts[sq]);
				if (BishopMagicAttacks[sq][index] == moves ||
				    BishopMagicAttacks[sq][index] == AllSquares) {
					BishopMagicAttacks[sq][index] = moves;
				} else {
					validMagic = false;
					break;
				}
				// Carry Rippler trick to traverse all possible occupancies (see
				// https://www.chessprogramming.org/Traversing_Subsets_of_a_Set)
				occupancy = (occupancy - relevantMoveMask) & relevantMoveMask;
			} while (occupancy);
		} while (!validMagic);
		// add candidateMagic if it is not an invalid magic
		BishopMagics[sq] = candidateMagic;
		// std::cout << "Rook Magics: square " << static_cast<Square>(sq) << " " << triedCount << " tries " <<
		// std::endl;
		totalTried += triedCount;
		triedCount = 0;
	}
	std::cout << "Magics: Total bishop tries " << totalTried << std::endl;
}

void
Magics::initRooks()
{
	// 1. For each square find a candidate magic
	// 2. For each possible occupancy, store attack vector in correct index, checking there is no collision
	// 3. If no collisions, we have valid magic
	std::mt19937 rng = std::mt19937(92);
	BitBoard candidateMagic;
	BitBoard relevantMoveMask;
	BitBoard occupancy;
	BitBoard moves;
	BitBoard index;
	BitBoard piece;
	bool validMagic;
	int triedCount;
	int totalTried = 0;
	for (int sq = 0; sq < numSquaresInBoard; ++sq) {
		triedCount = 0;
		// only care about relevant occupances (i.e. middle 4x4 when on edges and middle 5x5 when on corners)
		relevantMoveMask = HQRookAttack(static_cast<Square>(sq), NoSquares);
		piece = fromSq(static_cast<Square>(sq));
		for (int edge = 0; edge < numEdges; edge++) {
			if ((EdgeBitBoards[edge] & piece) == NoSquares)
				relevantMoveMask &= ~EdgeBitBoards[edge];
		}
		// assume valid until proven otherwise
		validMagic = true;
		// inside this loop, occupancy is equal to every possible relevant occupancy
		do {
			// part of Carry Rippler (see https://www.chessprogramming.org/Traversing_Subsets_of_a_Set)
			occupancy = 0ULL;
			validMagic = true; // assume valid till proven otherwise
			// find a random magic (see https://www.chessprogramming.org/Looking_for_Magics)
			candidateMagic = genRand(rng) & genRand(rng) & genRand(rng);

			if (++triedCount % 50000 == 0)
				std::cout << ".";
			// set rook magics to full board
			for (int b = 0; b < MagicRookMax; b++) {
				RookMagicAttacks[sq][b] = AllSquares;
			}
			do {
				moves = HQRookAttack(static_cast<Square>(sq), occupancy);
				index = (occupancy * candidateMagic) >> (64 - RookShifts[sq]);
				if (RookMagicAttacks[sq][index] == moves || RookMagicAttacks[sq][index] == AllSquares) {
					RookMagicAttacks[sq][index] = moves;
				} else {
					validMagic = false;
					break;
				}
				// Carry Rippler trick to traverse all possible occupancies (see
				// https://www.chessprogramming.org/Traversing_Subsets_of_a_Set)
				occupancy = (occupancy - relevantMoveMask) & relevantMoveMask;
			} while (occupancy);
		} while (!validMagic);
		// add candidateMagic if it is not an invalid magic
		RookMagics[sq] = candidateMagic;
		// std::cout << "Rook Magics: square " << static_cast<Square>(sq) << " " << triedCount << " tries " <<
		// std::endl;
		totalTried += triedCount;
		triedCount = 0;
	}
	std::cout << "Magics: Total rook tries " << totalTried << std::endl;
}

BitBoard
Magics::MagicBishopAttack(Square b, BitBoard occupancy) const
{
	int sq = static_cast<int>(b);
	return BishopMagicAttacks[sq][(occupancy * BishopMagics[sq]) >> (64 - BishopShifts[sq])];
}

BitBoard
Magics::MagicRookAttack(Square b, BitBoard occupancy) const
{
	int sq = static_cast<int>(b);
	return RookMagicAttacks[sq][(occupancy * RookMagics[sq]) >> (64 - RookShifts[sq])];
}

BitBoard
Magics::MagicQueenAttack(Square b, BitBoard occupancy) const
{
	return MagicBishopAttack(b, occupancy) | MagicRookAttack(b, occupancy);
}

BitBoard
HQBishopAttack(Square b, BitBoard occupancy)
{
	BitBoard piecePos = fromSq(b);
	BitBoard diagonal = AttackVectors::Diagonals[static_cast<int>(b)];
	BitBoard antiDiagonal = AttackVectors::AntiDiagonals[static_cast<int>(b)];
	BitBoard OccupiedInDiagonal = occupancy & diagonal;
	BitBoard OccupiedInAntiDiagonal = occupancy & antiDiagonal;
	// diagonal attacks
	BitBoard diagonalAttacks =
	  ((OccupiedInDiagonal - 2 * piecePos) ^ reverse((reverse(OccupiedInDiagonal) - 2 * reverse(piecePos)))) &
	  diagonal;
	// anti diagonal attacks
	BitBoard antiDiagonalAttacks = ((OccupiedInAntiDiagonal - 2 * piecePos) ^
					reverse((reverse(OccupiedInAntiDiagonal) - 2 * reverse(piecePos)))) &
				       antiDiagonal;
	return diagonalAttacks | antiDiagonalAttacks;
}
BitBoard
HQRookAttack(Square r, BitBoard occupancy)
{
	BitBoard piecePos = fromSq(r);
	BitBoard rank = RankBitBoards[static_cast<int>(rankFromSq(r))];
	BitBoard file = FileBitBoards[static_cast<int>(fileFromSq(r))];
	BitBoard OccupiedInRank = occupancy & rank;
	BitBoard OccupiedInFile = occupancy & file;
	// rank attacks
	BitBoard rankAttacks =
	  ((OccupiedInRank - 2 * piecePos) ^ reverse((reverse(OccupiedInRank) - 2 * reverse(piecePos)))) & rank;
	// file attacks
	BitBoard fileAttacks =
	  ((OccupiedInFile - 2 * piecePos) ^ reverse((reverse(OccupiedInFile) - 2 * reverse(piecePos)))) & file;
	return rankAttacks | fileAttacks;
}

BitBoard
HQQueenAttack(Square q, BitBoard occupancy)
{
	return HQRookAttack(q, occupancy) | HQBishopAttack(q, occupancy);
}

BitBoard
genRand(std::mt19937& rng)
{
	const BitBoard bottomTwoBytes = 0xFFFFULL;
	return (static_cast<BitBoard>(rng()) & bottomTwoBytes) |
	       ((static_cast<BitBoard>(rng()) & bottomTwoBytes) << 16) |
	       ((static_cast<BitBoard>(rng()) & bottomTwoBytes) << 32) |
	       ((static_cast<BitBoard>(rng()) & bottomTwoBytes) << 48);
}