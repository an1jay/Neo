#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "utils.h"
#include <iostream>

int
main()
{
	printRookMagicOccupancyMask();
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

void
HQMagicsDemo()
{
	Magics mag = Magics();
	Square s;
	BitBoard hq;
	BitBoard m;
	for (int sq = 0; sq < numSquaresInBoard; sq++) {
		s = static_cast<Square>(sq);
		hq = HQBishopAttack(s, NoSquares);
		m = mag.MagicBishopAttack(s, NoSquares);
		std::cout << s << std::endl;
		std::cout << "Hyperbola Quintessence" << std::endl;
		std::cout << asBoardString(hq);
		std::cout << "Magics" << std::endl;
		std::cout << asBoardString(m);
		std::cout << "----------------" << std::endl;
	}
}

void
HQvsMagics(BitBoard occ)
{
	Magics mag = Magics();
	Square s;
	BitBoard hq;
	BitBoard m;
	BitBoard maskedOcc = occ & AllSquares;

	for (int sq = 0; sq < numSquaresInBoard; sq++) {
		s = static_cast<Square>(sq);
		hq = HQBishopAttack(s, maskedOcc);
		m = mag.MagicBishopAttack(s, maskedOcc);
		std::cout << s << std::endl;
		std::cout << "Occupancy" << std::endl;
		std::cout << asBoardString(maskedOcc);
		std::cout << "Relevant Occupancy" << std::endl;
		std::cout << asBoardString(maskedOcc & HQBishopAttack(s, NoSquares));
		std::cout << "Hyperbola Quintessence" << std::endl;
		std::cout << asBoardString(hq);
		std::cout << "Magics" << std::endl;
		std::cout << asBoardString(m);
		std::cout << "----------------" << std::endl;
	}
}
