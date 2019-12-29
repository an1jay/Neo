#include "catch.h"

#include "constants.h"
#include "magics.h"

TEST_CASE("Magics", "[Magics]")
{
	Magics m = Magics();

	SECTION("Hyperbola Quintessence Bishop move generation")
	{
		Square s = Square::A1;
		REQUIRE(m.HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						    AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						   ~fromSq(s)));
		s = Square::F6;
		REQUIRE(m.HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						    AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						   ~fromSq(s)));
		s = Square::C3;
		REQUIRE(m.HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						    AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						   ~fromSq(s)));
		s = Square::D5;
		REQUIRE(m.HQBishopAttack(s, 0) == ((AttackVectors::Diagonals[static_cast<int>(s)] |
						    AttackVectors::AntiDiagonals[static_cast<int>(s)]) &
						   ~fromSq(s)));
	}
	SECTION("Hyperbola Quintessence Rook move generation")
	{
		Square s = Square::A1;
		REQUIRE(m.HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
		s = Square::F6;
		REQUIRE(m.HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
		s = Square::F4;
		REQUIRE(m.HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
		s = Square::F2;
		REQUIRE(m.HQRookAttack(s, 0) == AttackVectors::Rook[static_cast<int>(s)]);
	}
	SECTION("Magics Bishop move generation")
	{
		// TODO:
	}
	SECTION("Magics Rook move generation")
	{
		// TODO:
	}
}