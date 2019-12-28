#include "bitboard.h"
#include "constants.h"
#include "utils.h"
#include <iostream>

int main()
{

	std::cout << asBoardString(RankSecond) << std::endl;
	std::cout << asBitString(RankSecond) << std::endl;

	std::cout << asBoardString(AttackVectors::King[27]) << std::endl;
	std::cout << static_cast<Square>(27) << std::endl;

	return 0;
}