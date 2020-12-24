#include "constants.h"
#include "magics.h"
#include "types.h"
#include "utils.h"

#include <string>

#pragma once

struct StateInfo
{
	Key _posKey;
	int _rule50;
	Piece _capturedPiece;
	BitBoard _checkersBB;
	GameResult _gameResult;
	StateInfo* _previous;
};

struct Position
{
      public:
	Position();
	~Position();
	Position(const Position&) = delete;
	Position& operator=(const Position&) = delete;

	// TODO: FEN string input/output
	Position& SetfromFEN(const std::string& fenStr);
	const std::string GetFEN() const;

	// Position representation
	BitBoard PieceBB(PieceType pt) const;
	BitBoard ColorBB(Color c) const;
	BitBoard PieceBB(Color c, PieceType pt) const;
	Piece PieceOn(Square s) const;
	bool isSqEmpty(Square s) const;
	int PieceCount(PieceType pt) const;
	int PieceCount(PieceType pt, Color c) const;

	// Move properties
	bool isLegalMove(Ply p);
	bool isPseudoLegalMove(Ply p);
	bool isCapture(Ply p);

	// Do and undo moves
	void doMove(Ply p);
	void undoMove(Ply p);

	// For debugging purposes
	bool isValid() const;

      private:
	// ******* Data Members *******
	// Board Representation
	BitBoard _byColorBB[2];
	BitBoard _byPieceTypeBB[5];
	Piece _board[numSquaresInBoard];

	int _pieceCount[static_cast<int>(Piece::NB_NONE)];

	void movePiece(Square origin, Square destination);
	void removePiece(Square sq);
	void addPiece(Square sq, Piece p);
	BitBoard calculateCheckers();
	GameResult calculateGameResult();

	Magics _magics;
	// Position information
	Color _sideToMove;
	int _halfMoveClock;
	StateInfo* _st;
};

inline BitBoard
Position::PieceBB(PieceType pt) const
{
	return _byPieceTypeBB[static_cast<int>(pt)];
}

inline BitBoard
Position::ColorBB(Color c) const
{
	return _byColorBB[static_cast<int>(c)];
}

inline BitBoard
Position::PieceBB(Color c, PieceType pt) const
{
	return ColorBB(c) & PieceBB(pt);
}

inline Piece
Position::PieceOn(Square s) const
{
	return _board[static_cast<int>(s)];
}

inline bool
Position::isSqEmpty(Square s) const
{
	return PieceOn(s) == Piece::NB_NONE;
}

inline int
Position::PieceCount(PieceType pt, Color c) const
{
	return _pieceCount[static_cast<int>(pieceFromPieceTypeColor(pt, c))];
}

inline int
Position::PieceCount(PieceType pt) const
{
	return PieceCount(pt, Color::White) + PieceCount(pt, Color::Black);
}
