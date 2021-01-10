#include "position.h"

#include "bitboard.h"
#include "move_gen.h"
#include "terminal_colors.h"

#include <cassert>
#include <random>

Zobrist::Zobrist()
{
	std::mt19937 rng = std::mt19937(92);
	for (int sq = 0; sq < numSquaresInBoard; sq++)
		for (int p = 0; p < static_cast<int>(Piece::NB_NONE); p++)
			_pieceSqArray[sq][p] = genRand<Key>(rng);

	// _sideToMoveArray[static_cast<int>(Color::White)] = genRand<Key>(rng);
	// _sideToMoveArray[static_cast<int>(Color::Black)] = genRand<Key>(rng);
}

Key
Zobrist::getPieceKey(Piece p, Square s)
{
	assert(p != Piece::NB_NONE);
	return _pieceSqArray[static_cast<int>(s)][static_cast<int>(p)];
}

Position::Position()
{
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

	// **** Setup Zobrist Hash ****
	Key posHash = 0ULL;

	for (int sq = 0; sq < numSquaresInBoard; sq++)
		if (_board[sq] != Piece::NB_NONE)
			posHash ^= _zobristHasher.getPieceKey(_board[sq], static_cast<Square>(sq));

	_st = new StateInfo{
		posHash, 0, Piece::NB_NONE, NoSquares, GameResult::NB_NONE, nullptr,
	};
}

Position::~Position()
{
	std::vector<StateInfo*> sts;
	StateInfo* current_st = _st;

	while (current_st != nullptr) {
		sts.push_back(current_st);
		current_st = current_st->_previous;
	}

	for (StateInfo* st : sts)
		delete st;
}

bool
Position::isCapture(Ply p)
{
	Square destination = getDestSquare(p);
	return pieceOn(destination) != Piece::NB_NONE;
}

void
Position::movePiece(Square origin, Square destination)
{
	Piece p = pieceOn(origin);
	assert(p != Piece::NB_NONE);
	removePiece(origin);
	addPiece(destination, p);
}

void
Position::removePiece(Square sq)
{
	Piece to_be_removed = pieceOn(sq);
	if (to_be_removed == Piece::NB_NONE)
		return;
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
	// 4 things to update
	// - piece array (mailbox board)
	// - piece type BB
	// - color BB
	// - piece count
	_board[static_cast<int>(sq)] = p;
	if (p != Piece::NB_NONE) {
		_byPieceTypeBB[static_cast<int>(pieceTypeFromPiece(p))] ^= BBfromSq(sq);
		_byColorBB[static_cast<int>(colorFromPiece(p))] ^= BBfromSq(sq);
		_pieceCount[static_cast<int>(p)]++;
	}
}

BitBoard
Position::calculateCheckers()
{
	BitBoard ourKingBB = _byColorBB[static_cast<int>(_sideToMove)] &
			     _byPieceTypeBB[static_cast<int>(PieceType::King)];
	Square ourKingSq = sqFromBB(ourKingBB);

	// Given location of King:
	// 1) check if pieces exists
	// 2) calculate attacks from this location for: pawn (only captures), knight, rook, queen & king
	// 3) for these attacks check if the piece is present at those identified locations points
	// 4) if a piece is present, then this piece is a checker

	BitBoard checkers = NoSquares;
	BitBoard ourOccupancy = colorBB(_sideToMove);
	BitBoard theirOccupancy = colorBB(otherColor(_sideToMove));

	// Handle normal (i.e. symmetric attack) cases (i.e. not pawns)
	PieceType normalCases[] = {
		PieceType::Queen, PieceType::Rook, PieceType::Knight, PieceType::King
	};
	for (PieceType nc : normalCases) {

		if (pieceCount(nc, otherColor(_sideToMove)) > 0) {
			BitBoard attacks =
			  calculateAttackBB(nc, ourKingSq, ourOccupancy, theirOccupancy, _magics);
			checkers |= attacks & pieceBB(otherColor(_sideToMove), nc);
		}
	}

	// Handle pawns
	if (pieceCount(PieceType::Pawn, otherColor(_sideToMove)) > 0) {
		BitBoard pAttacks = AttackVectors::WhitePawnCapture[static_cast<int>(ourKingSq)];
		if (_sideToMove == Color::Black)
			pAttacks = AttackVectors::BlackPawnCapture[static_cast<int>(ourKingSq)];
		checkers |= pAttacks & pieceBB(otherColor(_sideToMove), PieceType::Pawn);
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

	assert(p != INVALID_PLY);

	const StateInfo* old_st = _st;

	const Square origin = getOriginSquare(p);
	const Square destination = getDestSquare(p);
	const PieceType promote = getPromoPieceType(p);

	int rule50clock = old_st->_rule50 + 1;
	_halfMoveClock++;

	Key posKey = old_st->_posKey;
	Piece capturedPiece = Piece::NB_NONE;
	if (isCapture(p)) {
		capturedPiece = pieceOn(destination);
		if (capturedPiece == Piece::W_King)
			std::cout << *this;
		posKey ^= _zobristHasher.getPieceKey(capturedPiece, destination);
		removePiece(destination);

		rule50clock = 0; // reset Rule 50 if capture or pawn move
	}
	if (pieceOn(origin) == pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove))
		rule50clock = 0;

	// move piece
	Piece movedPiece = pieceOn(origin);
	movePiece(origin, destination);
	posKey ^= _zobristHasher.getPieceKey(movedPiece, origin);
	posKey ^= _zobristHasher.getPieceKey(movedPiece, destination);

	// if there is a promotion, perform the promotion
	if (promote != PieceType::NB_NONE) {
		assert(pieceOn(destination) ==
		       pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove));
		posKey ^= _zobristHasher.getPieceKey(pieceOn(destination), destination);
		removePiece(destination);

		addPiece(destination, pieceFromPieceTypeColor(promote, _sideToMove));
		posKey ^= _zobristHasher.getPieceKey(pieceOn(destination), destination);
	}

	_sideToMove = otherColor(_sideToMove);
	BitBoard checkers = calculateCheckers();

	StateInfo* newSt = new StateInfo{
		posKey,   rule50clock,	 capturedPiece,
		checkers, GameResult::NB_NONE, const_cast<StateInfo*>(old_st),

	};
	_st = newSt;
	_st->_gameResult = calculateGameResult(); // GameResult calculation requires ST
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

	assert(p != INVALID_PLY);

	// Reverse various positions specific game state
	_sideToMove = otherColor(_sideToMove);
	_halfMoveClock--;

	// Move pieces
	const Square origin = getOriginSquare(p);
	const Square destination = getDestSquare(p);
	const PieceType promote = getPromoPieceType(p);

	// Move piece back
	movePiece(destination, origin);
	// Replace destination with captured piece (this is Piece::NB_NONE if no capture happened previously)
	addPiece(destination, _st->_capturedPiece);

	// if promotion, demote the piece to a pawn
	if (promote != PieceType::NB_NONE) {
		removePiece(origin);
		addPiece(origin, pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove));
	}

	const StateInfo* old_st = _st->_previous;
	// TODO - What to do with new ST? For now, delete it
	delete _st;
	_st = const_cast<StateInfo*>(old_st);
}

// Move search should not use this method - this is slow
GameResult
Position::calculateGameResult()
{
	std::vector<Ply> legalMoves = generateLegalPlies();
	const int numberOfLegalMoves = legalMoves.size();
	const bool inCheck = this->_st->_checkersBB != NoSquares;

	// Checkmate
	if (numberOfLegalMoves == 0 && inCheck)
		return (_sideToMove == Color::White) ? GameResult::BlackWin : GameResult::WhiteWin;

	// Stalemate
	if (numberOfLegalMoves == 0 && !inCheck)
		return GameResult::Draw;

	// Non stalemate draw -- 3 fold repetition / 50 move rule
	if (isNonStalemateDraw())
		return GameResult::Draw;

	// If not ended - game is still in play
	return GameResult::NB_NONE;
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

std::vector<Ply>
Position::generatePseudoLegalPlies()
{
	std::vector<Ply> plies;
	if (_st->_gameResult != GameResult::NB_NONE)
		return plies;

	BitBoard ourPieces = _byColorBB[static_cast<int>(_sideToMove)];
	Square pieceSq;
	Square destSq;
	Piece pc;
	BitBoard attackVector;

	const BitBoard ourOccupancy = _byColorBB[static_cast<int>(_sideToMove)];
	const BitBoard theirOccupancy = _byColorBB[static_cast<int>(otherColor(_sideToMove))];

	while (ourPieces != NoSquares) {
		// Find piece square and remove from bitboard of our pieces
		pieceSq = findMSB(ourPieces);
		ourPieces = ourPieces ^ BBfromSq(pieceSq);
		pc = pieceOn(pieceSq);
		attackVector =
		  calculateAttackBB(pc, pieceSq, ourOccupancy, theirOccupancy, _magics);

		while (attackVector != NoSquares) {
			// Find destination square and remove from attack vector
			destSq = findMSB(attackVector);
			attackVector = attackVector ^ BBfromSq(destSq);

			if ((pieceTypeFromPiece(pc) == PieceType::Pawn) &&
			    ((_sideToMove == Color::Black && rankFromSq(destSq) == Rank::First) ||
			     (_sideToMove == Color::White && rankFromSq(destSq) == Rank::Sixth))) {
				plies.push_back(encodePly(destSq, pieceSq, PieceType::Queen));
				plies.push_back(encodePly(destSq, pieceSq, PieceType::Rook));
				plies.push_back(encodePly(destSq, pieceSq, PieceType::Knight));
			} else
				plies.push_back(encodePly(destSq, pieceSq, PieceType::NB_NONE));
		}
	}
	return plies;
}

std::vector<Ply>
Position::generateLegalPlies()
{
	std::vector<Ply> pseudoLegalPlies = generatePseudoLegalPlies();

	// Vectors for various types of legal moves
	std::vector<Ply> promotionPlies;
	std::vector<Ply> capturePlies;
	std::vector<Ply> checkPlies;
	std::vector<Ply> otherPlies;

	bool isLegal;
	bool isCapture;
	bool givesCheck;

	for (Ply p : pseudoLegalPlies) {
		isLegal = isLegalPly(p, givesCheck, isCapture);

		if (isLegal) {
			if (getPromoPieceType(p) != PieceType::NB_NONE)
				promotionPlies.push_back(p);
			else if (isCapture)
				capturePlies.push_back(p);
			else if (givesCheck)
				checkPlies.push_back(p);
			else
				otherPlies.push_back(p);
		}
	}

	// Reserve memory in promotion plies & copy in other types of moves
	promotionPlies.reserve(promotionPlies.size() + capturePlies.size() + checkPlies.size() +
			       otherPlies.size());

	promotionPlies.insert(promotionPlies.end(), capturePlies.begin(), capturePlies.end());
	promotionPlies.insert(promotionPlies.end(), checkPlies.begin(), checkPlies.end());
	promotionPlies.insert(promotionPlies.end(), otherPlies.begin(), otherPlies.end());

	return promotionPlies;
}

bool
Position::isLegalPly(Ply p, bool& givesCheck, bool& isCapture)
{
	assert(p != INVALID_PLY);

	// Decode ply information
	const Square origin = getOriginSquare(p);
	const Square destination = getDestSquare(p);
	const PieceType promote = getPromoPieceType(p);

	// **** Make Move ****
	const Piece capturedPiece = pieceOn(destination);

	removePiece(destination);
	movePiece(origin, destination);
	if (promote != PieceType::NB_NONE) {
		removePiece(destination);
		addPiece(destination, pieceFromPieceTypeColor(promote, _sideToMove));
	}
	BitBoard checkers = calculateCheckers();

	// Calculate whether otherSide is in check (used for move ordering)
	const BitBoard movedPieceAttacks =
	  calculateAttackBB(pieceOn(destination),
			    destination,
			    _byColorBB[static_cast<int>(_sideToMove)],
			    _byColorBB[static_cast<int>(otherColor(_sideToMove))],
			    _magics);
	const BitBoard otherSideKingBB = pieceBB(otherColor(_sideToMove), PieceType::King);
	givesCheck = (movedPieceAttacks & otherSideKingBB) != NoSquares;

	// Calculate whether move is a capture (used for move ordering)
	// TODO think about ordering captures by least valuable piece capturing most valuable
	isCapture = capturedPiece != Piece::NB_NONE;

	// If we are in check after move, move is illegal
	const bool isLegal = checkers == NoSquares;

	// **** Un-Make Move ****
	movePiece(destination, origin);
	addPiece(destination, capturedPiece);
	if (promote != PieceType::NB_NONE) {
		removePiece(origin);
		addPiece(origin, pieceFromPieceTypeColor(PieceType::Pawn, _sideToMove));
	}

	return isLegal;
}

bool
Position::isNonStalemateDraw()
{
	// 3 types of non stalemate draw
	//  - 50 move rule
	//  - insufficient material (only K & 1 N left)
	//  - 3 fold repetition

	// 50 move rule easy to detect
	// note: 50 moves corresponds to 100 plies
	if (_st->_rule50 >= 50 * 2) // in theory it shouldn't be greater
		return true;

	// insufficient material
	bool insuff = ((_pieceCount[static_cast<int>(Piece::W_Pawn)] == 0 &&
			_pieceCount[static_cast<int>(Piece::B_Pawn)] == 0) &&
		       (_pieceCount[static_cast<int>(Piece::W_Rook)] == 0 &&
			_pieceCount[static_cast<int>(Piece::B_Rook)] == 0) &&
		       (_pieceCount[static_cast<int>(Piece::W_Queen)] == 0 &&
			_pieceCount[static_cast<int>(Piece::B_Queen)] == 0) &&
		       (_pieceCount[static_cast<int>(Piece::W_Knight)] <= 1 &&
			_pieceCount[static_cast<int>(Piece::B_Knight)] <= 1));

	if (insuff)
		return true;

	// 3 fold repetition
	int end = std::min(_st->_rule50, _halfMoveClock);
	if (end <= 4) // 3-fold rep requires at least 2.5 moves (i.e. 5 plies)
		return false;
	StateInfo* old_st = _st->_previous->_previous;
	int count = 0;
	for (int i = 3; i <= end; i += 2) {
		if (old_st->_posKey == _st->_posKey)
			count++;
		old_st = old_st->_previous->_previous;
	}

	if (count >= 2) // in theory it shouldn't be greater
		return true;

	return false;
}