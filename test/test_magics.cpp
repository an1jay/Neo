#include "catch.h"

#include "constants.h"
#include "magics.h"

TEST_CASE("Magics", "[Magics]")
{
	Magics m = Magics();
	SECTION("Hyperbola Quintessence Bishop move generation")
	{
		Square s = Square::A1;
		REQUIRE(HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						  AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						 ~fromSq(s)));
		s = Square::F6;
		REQUIRE(HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						  AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						 ~fromSq(s)));
		s = Square::C3;
		REQUIRE(HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						  AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						 ~fromSq(s)));
		s = Square::D5;
		REQUIRE(HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						  AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						 ~fromSq(s)));
	}
	SECTION("Hyperbola Quintessence Rook move generation")
	{
		Square s = Square::A1;
		REQUIRE(HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
		s = Square::F6;
		REQUIRE(HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
		s = Square::F4;
		REQUIRE(HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
		s = Square::F2;
		REQUIRE(HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
	}
	SECTION("Magics Bishop move generation")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			auto occ = GENERATE(1231233123, 23452345, 245632456);
			occ = occ & AllSquares;
			REQUIRE(HQBishopAttack(static_cast<Square>(sq), static_cast<BitBoard>(occ)) ==
				m.MagicBishopAttack(static_cast<Square>(sq), static_cast<BitBoard>(occ)));
		}
	}
	SECTION("Magics Rook move generation")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			auto occ = GENERATE(1231233123, 23452345, 245632456);
			occ = occ & AllSquares;
			REQUIRE(HQRookAttack(static_cast<Square>(sq), static_cast<BitBoard>(occ)) ==
				m.MagicRookAttack(static_cast<Square>(sq), static_cast<BitBoard>(occ)));
		}
	}
}