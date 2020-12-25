#include "position.h"

#include "bitboard.h"
#include "movegen.h"
#include "terminal_colors.h"

#include <cassert>

Position::Position()
{
	_st = new StateInfo{
		0ULL, 0, Piece::NB_NONE, NoSquares, GameResult::NotEnded, nullptr,

	};

	// **** Instantiate Piece List ****
	for (int sq = 0; sq < numSquaresInBoard; sq++)
		_board[sq] = Piece::NB_NONE;

	// White pieces
	_board[static_cast<int>(Square::A1)] = Piece::W_Rook;
	_board[static_cast<int>(Square::F1)] = Piece::W_Rook;

	_board[static_cast<int>(Square::B1)] = Piece::W_Knight;
	_board[static_cast<int>(Square::E1)] = Piece::W_Knight;

	_board[static_cast<int>(Square::C1)] = Piece::W_Queen;
	_board[static_cast<int>(Square::D1)] = Piece::W_King;

	_board[static_cast<int>(Square::A2)] = Piece::W_Pawn;
	_board[static_cast<int>(Square::B2)] = Piece::W_Pawn;
	_board[static_cast<int>(Square::C2)] = Piece::W_Pawn;
	_board[static_cast<int>(Square::D2)] = Piece::W_Pawn;
	_board[static_cast<int>(Square::E2)] = Piece::W_Pawn;
	_board[static_cast<int>(Square::F2)] = Piece::W_Pawn;

	// Black pieces
	_board[static_cast<int>(Square::A6)] = Piece::B_Rook;
	_board[static_cast<int>(Square::F6)] = Piece::B_Rook;

	_board[static_cast<int>(Square::B6)] = Piece::B_Knight;
	_board[static_cast<int>(Square::E6)] = Piece::B_Knight;

	_board[static_cast<int>(Square::C6)] = Piece::B_Queen;
	_board[static_cast<int>(Square::D6)] = Piece::B_King;

	_board[static_cast<int>(Square::A5)] = Piece::B_Pawn;
	_board[static_cast<int>(Square::B5)] = Piece::B_Pawn;
	_board[static_cast<int>(Square::C5)] = Piece::B_Pawn;
	_board[static_cast<int>(Square::D5)] = Piece::B_Pawn;
	_board[static_cast<int>(Square::E5)] = Piece::B_Pawn;
	_board[static_cast<int>(Square::F5)] = Piece::B_Pawn;

	_byColorBB[static_cast<int>(Color::White)] = NoSquares;
	_byColorBB[static_cast<int>(Color::Black)] = NoSquares;

	// **** Instantiate Piece Type BBs ****
	for (int pt = 0; pt < static_cast<int>(PieceType::NB_NONE); pt++)
		_byPieceTypeBB[pt] = NoSquares;

	// **** Instantiate Piece Count Array ****
	for (int p = 0; p < static_cast<int>(Piece::NB_NONE); p++)
		_pieceCount[p] = NoSquares;

	// **** Update other members ****
	updateMembersFromPieceList();
	_sideToMove = Color::White;
	_halfMoveClock = 0;
}

Position::~Position()
{
	// TODO
	delete _st;
}

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

	// 4 things to update
	// - piece array (mailbox board)
	// - piece type BB
	// - color BB
	// - piece count
	_board[static_cast<int>(sq)] = Piece::NB_NONE;
	_byPieceTypeBB[static_cast<int>(pieceTypeFromPiece(to_be_removed))] ^= BBfromSq(sq);
	_byColorBB[static_cast<int>(colorFromPiece(to_be_removed))] ^= BBfromSq(sq);
	_pieceCount[static_cast<int>(to_be_removed)]--;
}

void
Position::addPiece(Square sq, Piece p)
{
	assert(PieceOn(sq) == Piece::NB_NONE);

	// 4 things to update
	// - piece array (mailbox board)
	// - piece type BB
	// - color BB
	// - piece count
	_board[static_cast<int>(sq)] = p;
	_byPieceTypeBB[static_cast<int>(pieceTypeFromPiece(p))] ^= BBfromSq(sq);
	_byColorBB[static_cast<int>(colorFromPiece(p))] ^= BBfromSq(sq);
	_pieceCount[static_cast<int>(p)]++;
}

BitBoard
Position::calculateCheckers()
{
	BitBoard ourKingBB = _byColorBB[static_cast<int>(_sideToMove)] &
			     _byPieceTypeBB[static_cast<int>(PieceType::King)];
	Square ourKingSq = sqFromBB(ourKingBB);

	// Given location of King:
	// 1) check if pieces exists
	// 2) calculate attacks from this location for: pawn (only captures), knight, rook & queen (a king cannot another king)
	// 3) for these attacks check if the piece is present at those identified locations points
	// 4) if a piece is present, then this piece is a checker

	BitBoard checkers = NoSquares;
	BitBoard ourOccupancy = ColorBB(_sideToMove);
	BitBoard theirOccupancy = ColorBB(otherColor(_sideToMove));

	// Handle normal (i.e. symmetric attack) cases (i.e. not pawns)
	PieceType normalCases[] = { PieceType::Queen, PieceType::Rook, PieceType::Knight };
	for (PieceType nc : normalCases) {
		if (PieceCount(nc, otherColor(_sideToMove)) > 0) {
			BitBoard attacks =
			  calculateAttackBB(nc, ourKingSq, ourOccupancy, theirOccupancy, _magics);
			checkers |= attacks & PieceBB(otherColor(_sideToMove), nc);
		}
	}

	// Handle pawns
	if (PieceCount(PieceType::Pawn, otherColor(_sideToMove)) > 0) {
		BitBoard pAttacks = AttackVectors::WhitePawnCapture[static_cast<int>(ourKingSq)];
		if (_sideToMove == Color::Black)
			pAttacks = AttackVectors::BlackPawnCapture[static_cast<int>(ourKingSq)];
		checkers |= pAttacks & PieceBB(otherColor(_sideToMove), PieceType::Pawn);
	}

	return checkers;
}

void
Position::doPly(Ply p)
{
	// Things to be done
	// - move piece, remove old piece, promote (if necessary)
	// - create new stateinfo object & update various position properties
	//    - side to move
	//    - half move clock
	//    - checkers of the (now updated) side to move
	//    - check whether game is ended (i.e. checkmate, stalemate,
	//      insufficient material, 50 move rule, threefold repetition)

	StateInfo* old_st = _st;

	Square origin = getOriginSquare(p);
	Square destination = getDestSquare(p);
	PieceType promote = getPromoPieceType(p);

	int rule50clock = old_st->_rule50 + 1;
	_halfMoveClock++;

	// reset Rule 50 Clock if capture or pawn move
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
	GameResult _gameResult = calculateGameResult();

	StateInfo* newSt = new StateInfo{
		posKey, rule50clock, capturedPiece, checkers, _gameResult, old_st,

	};

	_st = newSt;
}

void
Position::undoPly(Ply p)
{
	// couple of things to be done
	// - remove moved piece, add back old piece, reverse promotion (if necessary)
	// - reset to old stateinfo object & update various position properties
	//    - side to move
	//    - half move clock
	//    - whether side to move is in check (use checkers from state info object)
	//    - check whether game is ended (i.e. checkmate, stalemate,
	//      insufficient material, 50 move rule, threefold repetition)

	StateInfo* old_st = _st->_previous;

	// TODO - What to do with new ST? For now, delete it
	delete _st;
	_st = old_st;

	// Reverse various positions specific game state
	_sideToMove = otherColor(_sideToMove);
	_halfMoveClock--;

	// Move pieces
	Square origin = getOriginSquare(p);
	Square destination = getDestSquare(p);
	PieceType promote = getPromoPieceType(p);

	// Move piece back
	movePiece(destination, origin);
	// Replace destination with captured piece (this is Piece::NB_NONE if no capture happened previously)
	addPiece(destination, old_st->_capturedPiece);

	// if promotion, demote the piece to a pawn
	if (promote != PieceType::NB_NONE) {
		removePiece(destination);
		addPiece(destination, pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove));
	}
}

GameResult
Position::calculateGameResult()
{
	// FIXME
	return GameResult::NotEnded;
}

void
Position::updateMembersFromPieceList()
{
	for (int sq = 0; sq < numSquaresInBoard; sq++) {
		Piece pieceOnSq = _board[sq];
		if (pieceOnSq != Piece::NB_NONE) {
			Color c = colorFromPiece(pieceOnSq);
			PieceType pt = pieceTypeFromPiece(pieceOnSq);

			_byColorBB[static_cast<int>(c)] |= BBfromSq(static_cast<Square>(sq));
			_byPieceTypeBB[static_cast<int>(pt)] |= BBfromSq(static_cast<Square>(sq));

			_pieceCount[static_cast<int>(pieceOnSq)]++;
		}
	}
}
