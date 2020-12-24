#include "utils.h"

#include "constants.h"
#include "magics.h"
#include "types.h"

#include <sstream>

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
			oss << " "
			    << mapper(sqFromFileRank(static_cast<File>(f), static_cast<Rank>(r)))
			    << " |";
		}
	}
	oss << std::endl << boardSep << std::endl << "    A   B   C   D   E   F" << std::endl;
	return oss.str();
}
