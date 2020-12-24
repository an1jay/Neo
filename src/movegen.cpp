#include "movegen.h"

#include "constants.h"
#include "magics.h"

BitBoard
calculateAttackBB(Piece p, Square sq, BitBoard ourOccupancy, BitBoard theirOccupancy, Magics& m)
{
	switch (p) {
		case Piece::W_Pawn:
			return (AttackVectors::WhitePawnPush[static_cast<int>(sq)] &
				~(ourOccupancy | theirOccupancy)) |
			       (AttackVectors::WhitePawnCapture[static_cast<int>(sq)] &
				theirOccupancy);
			break;

		case Piece::B_Pawn:
			return (AttackVectors::BlackPawnPush[static_cast<int>(sq)] &
				~(ourOccupancy | theirOccupancy)) |
			       (AttackVectors::BlackPawnCapture[static_cast<int>(sq)] &
				theirOccupancy);
			break;

		case Piece::W_Knight:
		case Piece::B_Knight:
			return AttackVectors::Knight[static_cast<int>(sq)] & ~ourOccupancy;
			break;

		case Piece::W_Rook:
		case Piece::B_Rook:
			return m.MagicRookAttack(sq, ourOccupancy | theirOccupancy) & ~ourOccupancy;
			break;

		case Piece::W_Queen:
		case Piece::B_Queen:
			return m.MagicQueenAttack(sq, ourOccupancy | theirOccupancy) &
			       ~ourOccupancy;
			break;

		case Piece::W_King:
		case Piece::B_King:
			return AttackVectors::King[static_cast<int>(sq)] & ~ourOccupancy;
			break;
	};
}
