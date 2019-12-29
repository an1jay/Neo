#include "utils.h"
#include "constants.h"
#include "types.h"
#include <cstring>

Square
sqFromFileRank(const File f, const Rank r)
{
	if (f == File::NONE && r == Rank::NONE)
		return Square::NONE;
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
	os << RankChars[static_cast<int>(rankFromSq(s))];
	os << FileChars[static_cast<int>(fileFromSq(s))];
	return os;
}

char*
boardPrint(std::function<char(Square s)> mapper)
{
	char* board = new char[14 * 7 + 1];
	int index = 0;
	for (int f = numSquaresInRankFile - 1; f >= 0; f--) {
		board[index++] = FileChars[f];
		// board[index++] = ' ';
		for (int r = 0; r < numSquaresInRankFile; r++) {
			board[index++] = ' ';
			board[index++] = mapper(sqFromFileRank(static_cast<File>(f), static_cast<Rank>(r)));
		}
		board[index++] = '\n';
	}
	board[index++] = ' ';
	for (int r = 0; r < numSquaresInRankFile; r++) {
		board[index++] = ' ';
		board[index++] = RankChars[r];
	}
	board[index++] = '\n';
	board[index++] = '\0';
	return board;
}