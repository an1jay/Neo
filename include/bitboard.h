#include "constants.h"
#include "types.h"

#include <string>

#pragma once

std::string
asBitString(const BitBoard);

// Generate BitBoard from Square
inline BitBoard
BBfromSq(const Square s)
{
	return static_cast<BitBoard>(1) << static_cast<int>(s);
}

// Checks whether provided square is occupied in provided BitBoard
inline bool
BBoccupiedSq(const BitBoard b, const Square s)
{
	return b & BBfromSq(s);
}

// Counts number of occupancies in provided BitBoard
inline int
popCount(const BitBoard b)
{
	return __builtin_popcountll(b);
}

// Returns inverted BitBoard (but still in 36 least significant bits)
BitBoard
reverse(const BitBoard);