#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "utils.h"
#include <iostream>

int
main()
{
	Square s = Square::E4;
	Magics m = Magics();

	std::cout << s << std::endl;

	BitBoard b = fromSq(s);
	std::cout << asBoardString(b);
	std::cout << asBoardString(m.HQBishopAttack(s, 0));

	return 0;
}