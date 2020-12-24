#include "position.h"

#include "bitboard.h"
#include "movegen.h"

#include <cassert>

bool
Position::isCapture(Ply p)
{
	Square destination = getDestSquare(p);
	return PieceOn(destination) != Piece::NB_NONE;
}

void
Position::movePiece(Square origin, Square destination)
{
	Piece p = PieceOn(origin);
	assert(p != Piece::NB_NONE);

	removePiece(origin);
	addPiece(destination, p);
}

void
Position::removePiece(Square sq)
{
	Piece to_be_removed = PieceOn(sq);
	assert(to_be_removed != Piece::NB_NONE);

	// 3 things to update
	// - piece array (mailbox board)
	// - piece type BB
	// - color BB
	_board[static_cast<int>(sq)] = Piece::NB_NONE;
	_byPieceTypeBB[static_cast<int>(pieceTypeFromPiece(to_be_removed))] ^= fromSq(sq);
	_byColorBB[static_cast<int>(colorFromPiece(to_be_removed))] ^= fromSq(sq);
}

void
Position::addPiece(Square sq, Piece p)
{
	assert(PieceOn(sq) == Piece::NB_NONE);

	// 3 things to update
	// - piece array (mailbox board)
	// - piece type BB
	// - color BB
	_board[static_cast<int>(sq)] = p;
	_byPieceTypeBB[static_cast<int>(pieceTypeFromPiece(p))] ^= fromSq(sq);
	_byColorBB[static_cast<int>(colorFromPiece(p))] ^= fromSq(sq);
}

BitBoard
Position::calculateCheckers()
{
	BitBoard kingBB = _byColorBB[static_cast<int>(_sideToMove)] &
			  _byPieceTypeBB[static_cast<int>(PieceType::King)];
	Square kingSq = sqFromBB(kingBB);

	// Given location of King:
	// 1) calculate attacks from this location for: pawn, knight, rook & queen (a king cannot
	// attack king) 2) for these attacks check if the piece is present at those identified
	// locations points 3) if a piece is present, then this piece is a checker
}

void
Position::doMove(Ply p)
{
	// couple of things to be done
	// - move piece, remove old piece, promote (if necessary)
	// - create new stateinfo object & update various position properties
	//    - side to move
	//    - half move clock
	//    - whether side to move is in check
	//    - check whether game is ended (i.e. checkmate, stalemate,
	//      insufficient material, 50 move rule, threefold repetition)

	StateInfo* old_st = _st;

	Square origin = getOriginSquare(p);
	Square destination = getDestSquare(p);
	PieceType promote = getPromoPieceType(p);

	int rule50clock = old_st->_rule50 + 1;

	Piece capturedPiece = Piece::NB_NONE;
	if (isCapture(p)) {
		capturedPiece = PieceOn(destination);
		rule50clock = 0;
	}

	if (PieceOn(origin) == pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove))
		rule50clock = 0;

	// move piece & if there is a promotion, perform the promotion
	movePiece(origin, destination);
	if (promote != PieceType::NB_NONE) {
		removePiece(destination);
		addPiece(destination, pieceFromPieceTypeColor(promote, _sideToMove));
	}

	Key posKey = 0ULL;
	// TODO do hashing

	_sideToMove = otherColor(_sideToMove);
	BitBoard checkers = calculateCheckers();

	StateInfo* newSt = new StateInfo{
		posKey, rule50clock, capturedPiece, checkers, old_st,

	};

	_st = newSt;
	_halfMoveClock++;
	_gameResult = calculateGameResult();

	if (checkers == 0ULL)
		_inCheck = false;
	else
		_inCheck = true;
}

void
Position::undoMove(Ply p)
{
	// couple of things to be done
	// - remove moved piece, add back old piece, reverse promotion (if
	// necessary)
	// - reset to old stateinfo object & update various position properties
	//    - side to move
	//    - half move clock
	//    - whether side to move is in check (use checkers from state info
	//    object)
	//    - check whether game is ended (i.e. checkmate, stalemate,
	//      insufficient material, 50 move rule, threefold repetition)

	StateInfo* old_st = _st->_previous;

	// TODO - What to do with new ST?
	// for now delete it
	delete _st;
	_st = old_st;

	// Reverse various positions specific game state
	_sideToMove = otherColor(_sideToMove);
	_halfMoveClock--;
	if (old_st->_checkersBB == 0ULL)
		_inCheck = false;
	else
		_inCheck = true;

	// Move pieces
	Square origin = getOriginSquare(p);
	Square destination = getDestSquare(p);
	PieceType promote = getPromoPieceType(p);

	// Move piece back
	movePiece(destination, origin);
	// Replace destination with captured piece
	// (this is Piece::NB_NONE if no capture happened previously)
	addPiece(destination, old_st->_capturedPiece);

	// if promotion, demote the piece to a pawn
	if (promote != PieceType::NB_NONE) {
		removePiece(destination);
		addPiece(destination, pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove));
	}
}