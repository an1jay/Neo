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
	friend std::ostream& operator<<(std::ostream& os, Position& p);

	// Position representation
	BitBoard PieceBB(PieceType pt) const;
	BitBoard PieceBB(Color c, PieceType pt) const;
	BitBoard ColorBB(Color c) const;
	Piece PieceOn(Square s) const;
	bool isSqEmpty(Square s) const;
	int PieceCount(PieceType pt) const;
	int PieceCount(PieceType pt, Color c) const;

	std::vector<Ply>& generatePseudoLegalPlies();

	// Move properties
	bool isLegalPly(Ply p);
	bool isPseudoLegalPly(Ply p);
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

	void movePiece(Square origin, Square destination);
	void removePiece(Square sq);
	void addPiece(Square sq, Piece p);
	BitBoard calculateCheckers();
	GameResult calculateGameResult();
	void updateMembersFromPieceList();
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
