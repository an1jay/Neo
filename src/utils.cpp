#include "utils.h"
#include "constants.h"
#include "magics.h"
#include "types.h"
#include <sstream>

Square
sqFromFileRank(const File f, const Rank r)
{
	if (f == File::NB_NONE || r == Rank::NB_NONE)
		return Square::NB_NONE;
	else
		return static_cast<Square>(static_cast<int>(r) * numSquaresInRankFile + static_cast<int>(f));
}

File
fileFromSq(const Square s)
{
	return static_cast<File>(static_cast<int>(s) % numSquaresInRankFile);
}

Rank
rankFromSq(const Square s)
{
	return static_cast<Rank>(static_cast<int>(s) / numSquaresInRankFile);
}

std::ostream&
operator<<(std::ostream& os, Square s)
{
	os << FileChars[static_cast<int>(fileFromSq(s))];
	os << RankChars[static_cast<int>(rankFromSq(s))];
	return os;
}

std::string
boardPrint(std::function<char(Square s)> mapper)
{
	std::ostringstream oss;
	const char* boardSep = "  |---|---|---|---|---|---|";

	for (int r = numSquaresInRankFile - 1; r >= 0; r--) {
		oss << std::endl << boardSep << std::endl;
		oss << RankChars[r] << " |";
		for (int f = 0; f < numSquaresInRankFile; f++) {
			oss << " " << mapper(sqFromFileRank(static_cast<File>(f), static_cast<Rank>(r))) << " |";
		}
	}
	oss << std::endl << boardSep << std::endl << "    A   B   C   D   E   F" << std::endl;
	return oss.str();
}

Piece
pieceFromPieceTypeColor(const PieceType pt, const Color c)
{
	if (pt == PieceType::NB_NONE || c == Color::NB_NONE)
		return Piece::NB_NONE;
	return static_cast<Piece>(static_cast<int>(pt) * (static_cast<int>(c) + 1));
}

PieceType
pieceTypeFromPiece(const Piece p)
{
	if (p == Piece::NB_NONE)
		return PieceType::NB_NONE;
	return static_cast<PieceType>(static_cast<int>(p) % static_cast<int>(Piece::B_Pawn));
}

Color
colorFromPiece(const Piece p)
{
	if (p == Piece::NB_NONE)
		return Color::NB_NONE;
	if (static_cast<int>(p) <= static_cast<int>(Piece::W_King))
		return Color::White;
	return Color::Black;
}