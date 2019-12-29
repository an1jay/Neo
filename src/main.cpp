#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "utils.h"
#include <iostream>

int main()
{
	Square s = Square::A1;
	Magics m = Magics();

	std::cout << s << std::endl;

	BitBoard b = fromSq(s);
	std::cout << asBoardString(b) << std::endl;

	std::cout << asBoardString(m.HQBishopAttack(s, 0)) << std::endl;
	std::cout << asBoardString(m.HQRookAttack(s, 0)) << std::endl;

	return 0;
}