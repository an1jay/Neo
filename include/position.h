#include "constants.h"
#include "magics.h"
#include "types.h"
#include "utils.h"

#include <string>

#pragma once

struct StateInfo
{
      public:
	Key _posKey;
	int _rule50;
	Piece _capturedPiece;
	BitBoard _checkersBB;
	GameResult _gameResult;
	StateInfo* _previous;

	StateInfo() = default;
	// ~StateInfo();
};

struct Zobrist
{
      public:
	Zobrist();
	~Zobrist() = default;

	Zobrist(const Zobrist&) = delete;
	Zobrist& operator=(const Zobrist&) = delete;

	Key getPieceKey(Piece p, Square s);

      private:
	Key _pieceSqArray[static_cast<int>(Square::NB_NONE)][static_cast<int>(Piece::NB_NONE)];
	// Key _sideToMoveArray[static_cast<int>(Color::NB_NONE)];
};

struct Position
{
      public:
	Position();
	~Position();
	Position(const Position&) = delete;
	Position& operator=(const Position&) = delete;

	// TODO: FEN string input/output
	Position& setfromFEN(const std::string& fenStr);
	const std::string getFEN() const;
	friend std::ostream& operator<<(std::ostream& os, Position& p);

	// Position representation
	BitBoard pieceBB(PieceType pt) const;
	BitBoard pieceBB(Color c, PieceType pt) const;
	BitBoard colorBB(Color c) const;
	Piece pieceOn(Square s) const;
	bool isSqEmpty(Square s) const;
	int pieceCount(PieceType pt) const;
	int pieceCount(PieceType pt, Color c) const;

	std::vector<Ply> generatePseudoLegalPlies();
	std::vector<Ply> generateLegalPlies();

	// Move properties
	bool isLegalPly(Ply p); // TODO
	bool isCapture(Ply p);

	// Do and undo plies
	void doPly(Ply p);
	void undoPly(Ply p);

	// For debugging purposes
	bool isValid() const;

      private:
	// ******* Data Members *******
	// Board representation
	BitBoard _byColorBB[2];
	BitBoard _byPieceTypeBB[5];
	Piece _board[numSquaresInBoard];
	int _pieceCount[static_cast<int>(Piece::NB_NONE)];
	// Other info
	Color _sideToMove;
	int _halfMoveClock;
	StateInfo* _st; // TODO TBD whether this needs to be heap allocated

	Magics _magics;
	Zobrist _zobristHasher;

	void movePiece(Square origin, Square destination);
	void removePiece(Square sq);
	void addPiece(Square sq, Piece p);
	BitBoard calculateCheckers();
	GameResult calculateGameResult();
	void updateMembersFromPieceList();
	bool isNonStalemateDraw();
};

inline BitBoard
Position::pieceBB(PieceType pt) const
{
	return _byPieceTypeBB[static_cast<int>(pt)];
}

inline BitBoard
Position::colorBB(Color c) const
{
	return _byColorBB[static_cast<int>(c)];
}

inline BitBoard
Position::pieceBB(Color c, PieceType pt) const
{
	return colorBB(c) & pieceBB(pt);
}

inline Piece
Position::pieceOn(Square s) const
{
	return _board[static_cast<int>(s)];
}

inline bool
Position::isSqEmpty(Square s) const
{
	return pieceOn(s) == Piece::NB_NONE;
}

inline int
Position::pieceCount(PieceType pt, Color c) const
{
	return _pieceCount[static_cast<int>(pieceFromPieceTypeColor(pt, c))];
}

inline int
Position::pieceCount(PieceType pt) const
{
	return pieceCount(pt, Color::White) + pieceCount(pt, Color::Black);
}
