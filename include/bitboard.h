#include "types.h"

#pragma once

char *asBitString(const BitBoard b);

char *asBoardString(const BitBoard b);

// Checks whether provided square is occupied in provided BitBoard
inline bool occupiedSq(const BitBoard b, const Square s)
{
	return b & static_cast<BitBoard>(1) << static_cast<int>(s);
}

// Counts number of occupancies in provided BitBoard
inline bool popCount(const BitBoard b)
{
	return __builtin_popcountll(b);
}
