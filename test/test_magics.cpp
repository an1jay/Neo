#include "catch.h"

#include "constants.h"
#include "magics.h"

TEST_CASE("Magics", "[Magics]")
{
	Magics m = Magics();

	SECTION("Hyperbola Quintessence move generation")
	{
		Square s = Square::A1;
		REQUIRE(m.HQBishopAttack(s, 0) == (AttackVectors::Diagonals[static_cast<int>(s)] & ~fromSq(s)));
	}
}