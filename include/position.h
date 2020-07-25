#include <string>

#include "constants.h"
#include "types.h"
#include "utils.h"

#pragma once

struct StateInfo
{
	Key posKey;
	double nonPawnMaterial[static_cast<int>(Color::NB_NONE)];
	Score PSQT;
	

};

struct Position
{
      public:
	Position() = default;
	Position(const Position&) = delete;
	Position& operator=(const Position&) = delete;

	// TODO: FEN string input/output
	Position& SetfromFEN(const std::string& fenStr);
	const std::string GetFEN() const;

	// Position representation
	BitBoard PieceBB(PieceType pt) const;
	BitBoard PieceBB(Color c) const;
	BitBoard PieceBB(Color c, PieceType pt) const;
	Piece PieceOn(Square s) const;
	bool isSqEmpty(Square s) const;
	int PieceCount(PieceType pt, Color c) const;
	int PieceCount(PieceType pt) const;

	// Position properties
	Color sideToMove() const;

	// Do and undo moves
	void do_move();
	void undo_move();

	// For debugging purposes
	bool isValid() const;

      private:
	// ******* Data Members *******
	// Board Representation
	BitBoard _byColorBB[2];
	BitBoard _byPieceTypeBB[5];
	Piece _board[numSquaresInBoard];

	int _pieceCount[static_cast<int>(Piece::NB_NONE)];

	// Position information
	Color _sideToMove;
	int _halfMoveClock;
	bool _inCheck;
};

inline BitBoard
Position::PieceBB(PieceType pt) const
{
	return _byPieceTypeBB[static_cast<int>(pt)];
}

inline BitBoard
Position::PieceBB(Color c) const
{
	return _byColorBB[static_cast<int>(c)];
}

inline BitBoard
Position::PieceBB(Color c, PieceType pt) const
{
	return PieceBB(c) & PieceBB(pt);
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

inline Color
Position::sideToMove() const
{
	return _sideToMove;
}