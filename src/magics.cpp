#include "magics.h"
#include "constants.h"
#include "utils.h"

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
{}

void
Magics::initRooks()
{}

BitBoard
Magics::HQBishopAttack(Square b, BitBoard occ)
{
	BitBoard piecePos = fromSq(b);
	BitBoard diagonal = AttackVectors::Diagonals[static_cast<int>(b)];
	BitBoard antiDiagonal = AttackVectors::AntiDiagonals[static_cast<int>(b)];
	BitBoard OccupiedInDiagonal = occ & diagonal;
	BitBoard OccupiedInAntiDiagonal = occ & antiDiagonal;
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
Magics::HQRookAttack(Square r, BitBoard occ)
{
	BitBoard piecePos = fromSq(r);
	BitBoard rank = RankBitBoards[static_cast<int>(rankFromSq(r))];
	BitBoard file = FileBitBoards[static_cast<int>(fileFromSq(r))];
	BitBoard OccupiedInRank = occ & rank;
	BitBoard OccupiedInFile = occ & file;
	// rank attacks
	BitBoard rankAttacks =
	  ((OccupiedInRank - 2 * piecePos) ^ reverse((reverse(OccupiedInRank) - 2 * reverse(piecePos)))) & rank;
	// file attacks
	BitBoard fileAttacks =
	  ((OccupiedInFile - 2 * piecePos) ^ reverse((reverse(OccupiedInFile) - 2 * reverse(piecePos)))) & file;
	return rankAttacks | fileAttacks;
}