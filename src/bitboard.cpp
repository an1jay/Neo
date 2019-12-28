#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "utils.h"

char *asBitString(const BitBoard b)
{
	char *ret = new char[numSquaresInBoard + 1];
	int index = 0;
	for (int sq = numSquaresInBoard - 1; sq >= 0; sq--)
	{
		if (occupiedSq(b, static_cast<Square>(sq)))
			ret[index++] = '1';
		else
			ret[index++] = '0';
	}
	ret[index++] = '\0';
	return ret;
}

char *asBoardString(const BitBoard b)
{
	auto mapper = [&b](Square s) -> char {
		if (occupiedSq(b, s))
			return 'O';
		return ' ';
	};
	return boardPrint(mapper);
}