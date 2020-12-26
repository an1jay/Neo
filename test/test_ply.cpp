#include "catch.h"
// include catch first
#include "movegen.h"
#include "types.h"

TEST_CASE("Plies", "[Ply]")
{
	SECTION("encoding & decoding a Ply")
	{
		Ply pl1 = encodePly(Square::A5, Square::F6, PieceType::NB_NONE);
		REQUIRE(getDestSquare(pl1) == Square::A5);
		REQUIRE(getOriginSquare(pl1) == Square::F6);
		REQUIRE(getPromoPieceType(pl1) == PieceType::NB_NONE);

		Ply pl2 = encodePly(Square::A1, Square::E5, PieceType::Knight);
		REQUIRE(getDestSquare(pl2) == Square::A1);
		REQUIRE(getOriginSquare(pl2) == Square::E5);
		REQUIRE(getPromoPieceType(pl2) == PieceType::Knight);

		Ply pl3 = encodePly(Square::F6, Square::A1, PieceType::Queen);
		REQUIRE(getDestSquare(pl3) == Square::F6);
		REQUIRE(getOriginSquare(pl3) == Square::A1);
		REQUIRE(getPromoPieceType(pl3) == PieceType::Queen);
	}
}