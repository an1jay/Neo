#include "bitboard.h"
#include "constants.h"
#include "magics.h"
#include "position.h"
#include "utils.h"

#include <iostream>

int
main()
{
	// printRookMagicOccupancyMask();
	// std::cout << asBoardString(AllSquares) << std::endl;
	Position p;

	for (int i = 0; i < 25; i++) {
		std::cout << i;
		std::cout << p;

		std::cout << "Pseudo" << std::endl;
		auto pseudomoveList = p.generatePseudoLegalPlies();
		for (auto m : pseudomoveList)
			std::cout << asPlyString(m) << " | ";
		std::cout << std::endl;

		std::cout << "Legal" << std::endl;
		auto moveList = p.generateLegalPlies();
		for (auto m : moveList)
			std::cout << asPlyString(m) << " | ";

		std::cout << std::endl;
		p.doPly(moveList[0]);
	}
	return 0;
}