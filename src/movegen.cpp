#include "movegen.h"

#include "constants.h"
#include "magics.h"
#include "utils.h"

#include <cassert>

BitBoard
calculateAttackBB(Piece p, Square sq, BitBoard ourOccupancy, BitBoard theirOccupancy, Magics& m)
{
	assert(p != Piece::NB_NONE);
	BitBoard attackVector = NoSquares;
	switch (p) {
		case Piece::W_Pawn:
			assert(rankFromSq(sq) != Rank::First);
			assert(rankFromSq(sq) != Rank::Sixth);
			attackVector =
			  (AttackVectors::WhitePawnPush[static_cast<int>(sq)] &
			   ~(ourOccupancy | theirOccupancy)) |
			  (AttackVectors::WhitePawnCapture[static_cast<int>(sq)] & theirOccupancy);
			break;

		case Piece::B_Pawn:
			assert(rankFromSq(sq) != Rank::First);
			assert(rankFromSq(sq) != Rank::Sixth);
			attackVector =
			  (AttackVectors::BlackPawnPush[static_cast<int>(sq)] &
			   ~(ourOccupancy | theirOccupancy)) |
			  (AttackVectors::BlackPawnCapture[static_cast<int>(sq)] & theirOccupancy);
			break;

		case Piece::W_Knight:
		case Piece::B_Knight:
			attackVector = AttackVectors::Knight[static_cast<int>(sq)] & ~ourOccupancy;
			break;

		case Piece::W_Rook:
		case Piece::B_Rook:
			attackVector =
			  m.MagicRookAttack(sq, ourOccupancy | theirOccupancy) & ~ourOccupancy;
			break;

		case Piece::W_Queen:
		case Piece::B_Queen:
			attackVector =
			  m.MagicQueenAttack(sq, ourOccupancy | theirOccupancy) & ~ourOccupancy;
			break;

		case Piece::W_King:
		case Piece::B_King:
			attackVector = AttackVectors::King[static_cast<int>(sq)] & ~ourOccupancy;
			break;
	};
	return attackVector;
}

BitBoard
calculateAttackBB(PieceType p, Square sq, BitBoard ourOccupancy, BitBoard theirOccupancy, Magics& m)
{
	assert(p != PieceType::Pawn);
	return calculateAttackBB(
	  pieceFromPieceTypeColor(p, Color::White), sq, ourOccupancy, theirOccupancy, m);
}