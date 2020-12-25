#include "bitboard.h"

#include "constants.h"
#include "types.h"
#include "utils.h"

#include <sstream>

std::string
asBitString(const BitBoard b)
{
	std::ostringstream oss;
	for (int sq = numSquaresInBoard - 1; sq >= 0; sq--) {
		if (BBoccupiedSq(b, static_cast<Square>(sq)))
			oss << '1';
		else
			oss << '0';
	}
	return oss.str();
}

BitBoard
reverse(const BitBoard b)
{

	BitBoard rev = b;
	rev = ((rev >> 1) & ((BitBoard)0x55555555 << 32 | 0x55555555)) |
	      ((rev << 1) & ((BitBoard)0xaaaaaaaa << 32 | 0xaaaaaaaa));
	rev = ((rev >> 2) & ((BitBoard)0x33333333 << 32 | 0x33333333)) |
	      ((rev << 2) & ((BitBoard)0xcccccccc << 32 | 0xcccccccc));
	rev = ((rev >> 4) & ((BitBoard)0x0f0f0f0f << 32 | 0x0f0f0f0f)) |
	      ((rev << 4) & ((BitBoard)0xf0f0f0f0 << 32 | 0xf0f0f0f0));
	rev = ((rev >> 8) & ((BitBoard)0x00ff00ff << 32 | 0x00ff00ff)) |
	      ((rev << 8) & ((BitBoard)0xff00ff00 << 32 | 0xff00ff00));
	rev = ((rev >> 16) & ((BitBoard)0x0000ffff << 32 | 0x0000ffff)) |
	      ((rev << 16) & ((BitBoard)0xffff0000 << 32 | 0xffff0000));
	rev = (rev >> 32) | (rev << 32);
	return rev >> numSquaresShiftFactor;
}