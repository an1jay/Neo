#include "utils.h"

#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "move_gen.h"
#include "position.h"
#include "terminal_colors.h"
#include "types.h"

#include <sstream>
#include <utility>

bool
plyInList(Ply p, std::vector<Ply> plyList)
{
	for (Ply canPly : plyList)
		if (canPly == p)
			return true;
	return false;
}

std::ostream&
operator<<(std::ostream& os, Square s)
{
	if (s == Square::NB_NONE)
		os << "SquareNone";
	else {
		os << FileChars[static_cast<int>(fileFromSq(s))];
		os << RankChars[static_cast<int>(rankFromSq(s))];
	}
	return os;
}

std::ostream&
operator<<(std::ostream& os, Color c)
{
	switch (c) {
		case Color::White:
			os << WHTIO("White");
			break;
		case Color::Black:
			os << BLKIO("Black");
			break;
		case Color::NB_NONE:
			os << "ColorNone";
			break;
	}
	return os;
}

std::ostream&
operator<<(std::ostream& os, Piece p)
{
	if (p == Piece::NB_NONE)
		os << "PieceNone";
	else {
		if (colorFromPiece(p) == Color::White)
			os << WHTIO(PieceChars[static_cast<int>(p)]);
		else if (colorFromPiece(p) == Color::Black)
			os << BLKIO(PieceChars[static_cast<int>(p)]);
	}
	return os;
}

std::ostream&
operator<<(std::ostream& os, GameResult gr)
{
	switch (gr) {
		case GameResult::NB_NONE:
			os << "Not Ended";
			break;
		case GameResult::Draw:
			os << "Draw";
			break;
		case GameResult::WhiteWin:
			os << WHTIO("WhiteWin");
			break;
		case GameResult::BlackWin:
			os << BLKIO("BlackWin");
			break;
	}
	return os;
}

std::ostream&
operator<<(std::ostream& os, PieceType pt)
{
	if (pt == PieceType::NB_NONE)
		os << "PieceTypeNone";
	else
		os << EMPHIO(PieceTypeChars[static_cast<int>(pt)]);
	return os;
}

std::ostream&
operator<<(std::ostream& os, Position& p)
{
	auto mapper = [&p](Square s) -> std::pair<char, Color> {
		Piece pc = p.pieceOn(s);
		return std::pair<char, Color>{ PieceChars[static_cast<int>(pc)],
					       colorFromPiece(pc) };
	};
	os << boardPrint(mapper);
	os << std::endl << " ---------------------------" << std::endl;
	os << EMPHIO("  To move:        ") << p._sideToMove << std::endl
	   << EMPHIO("  In check:       ") << ((p._st->_checkersBB != NoSquares) ? "true" : "false")
	   << std::endl
	   << EMPHIO("  No of plies:    ") << p._halfMoveClock << std::endl
	   << EMPHIO("  Game result:    ") << p._st->_gameResult << std::endl
	   << EMPHIO("  Captured piece: ") << p._st->_capturedPiece << std::endl
	   << EMPHIO("  Rule 50 plies:  ") << p._st->_rule50 << std::endl;
	os << " ---------------------------" << std::endl << std::endl;
	return os;
}

std::string
asBoardString(const BitBoard b)
{
	auto mapper = [&b](Square s) -> std::pair<char, Color> {
		if (BBoccupiedSq(b, s))
			return std::pair<char, Color>('X', Color::NB_NONE);
		return std::pair<char, Color>(' ', Color::NB_NONE);
	};
	return boardPrint(mapper);
}

std::string
asPlyString(const Ply p)
{
	std::ostringstream oss;
	if (getPromoPieceType(p) != PieceType::NB_NONE)
		oss << getOriginSquare(p) << getDestSquare(p) << "=" << getPromoPieceType(p);
	else
		oss << getOriginSquare(p) << getDestSquare(p);
	return oss.str();
}

std::string
boardPrint(std::function<std::pair<char, Color>(Square s)> mapper)
{
	std::ostringstream oss;
	const char* boardSep = "  -|---|---|---|---|---|---|";
	Color c;
	char piece;

	for (int r = numSquaresInRankFile - 1; r >= 0; r--) {
		oss << std::endl << boardSep << std::endl;
		oss << " " << RankChars[r] << " |";
		for (int f = 0; f < numSquaresInRankFile; f++) {
			const auto& c_piece =
			  mapper(sqFromFileRank(static_cast<File>(f), static_cast<Rank>(r)));
			piece = c_piece.first;
			c = c_piece.second;

			if (c == Color::White)
				oss << " " << WHTIO(piece) << " |";
			else if (c == Color::Black)
				oss << " " << BLKIO(piece) << " |";
			else
				oss << " " << piece << " |";
		}
	}
	oss << std::endl << boardSep << std::endl << "     A   B   C   D   E   F" << std::endl;
	return oss.str();
}
