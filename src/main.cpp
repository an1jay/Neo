#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "utils.h"
#include <iostream>

int main()
{
	Square s = Square::C3;
	Magics m = Magics();

	std::cout << s << std::endl;

	BitBoard b = fromSq(s);
	std::cout << asBoardString(b) << std::endl;

	std::cout << asBoardString(m.HQBishopAttacks(s, 0)) << std::endl;
	std::cout << asBoardString(m.HQRookAttacks(s, 0)) << std::endl;

	return 0;
}