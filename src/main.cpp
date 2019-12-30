#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "utils.h"
#include <iostream>

int
main()
{
	Magics mag = Magics();
	// Square s;
	// BitBoard hq;
	// BitBoard m;
	// for (int sq = 0; sq < numSquaresInBoard; sq++) {
	// 	s = static_cast<Square>(sq);
	// 	hq = mag.HQBishopAttack(s, NoSquares);
	// 	m = mag.MagicBishopAttack(s, NoSquares);
	// 	std::cout << s << std::endl;
	// 	std::cout << "Hyperbola Quintessence" << std::endl;
	// 	std::cout << asBoardString(hq);
	// 	std::cout << "Magics" << std::endl;
	// 	std::cout << asBoardString(m);
	// 	std::cout << "----------------" << std::endl;
	// }
	return 0;
}

void
HQDemo()
{
	Magics m = Magics();
	Square s;
	BitBoard b;
	for (int sq = 0; sq < numSquaresInBoard; sq++) {
		s = static_cast<Square>(sq);
		b = HQRookAttack(s, 0);
		std::cout << s << "  " << popCount(b) << std::endl;
		std::cout << asBoardString(b);
		std::cout << "----------------" << std::endl;
	}
}