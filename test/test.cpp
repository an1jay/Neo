#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "bitboard.h"
// #include "constants.h"
// #include "types.h"

TEST_CASE("BitBoards", "[BitBoard]")
{
	SECTION("generating a BitBoard from a Square")
	{
		REQUIRE(fromSq(Square::D5) == static_cast<BitBoard>(1) << static_cast<int>(Square::D5));
		REQUIRE(fromSq(Square::A4) == static_cast<BitBoard>(1) << static_cast<int>(Square::A4));
		REQUIRE(fromSq(Square::B5) == static_cast<BitBoard>(1) << static_cast<int>(Square::B5));
		REQUIRE(fromSq(Square::F5) == static_cast<BitBoard>(1) << static_cast<int>(Square::F5));
	}

	SECTION("counting ones in a BitBoard")
	{
		BitBoard b = fromSq(Square::D5);
		REQUIRE(popCount(b) == 1);
		b |= fromSq(Square::E5);
		REQUIRE(popCount(b) == 2);
		b |= fromSq(Square::A5);
		REQUIRE(popCount(b) == 3);
		b |= fromSq(Square::C3);
		REQUIRE(popCount(b) == 4);
	}

	SECTION("checking whether a Square is occupied in a BitBoard")
	{
		Square s = Square::D5;
		REQUIRE(occupiedSq(fromSq(s), s));
		s = Square::E5;
		REQUIRE(occupiedSq(fromSq(s), s));
		s = Square::F5;
		REQUIRE(occupiedSq(fromSq(s), s));
		s = Square::A1;
		REQUIRE(occupiedSq(fromSq(s), s));
	}

	SECTION("checking whether reversing a BitBoard works")
	{
		Square s = Square::D5;
		REQUIRE(reverse(fromSq(s)) == fromSq(static_cast<Square>(numSquaresInBoard - 1 - static_cast<int>(s))));
		s = Square::D3;
		REQUIRE(reverse(fromSq(s)) == fromSq(static_cast<Square>(numSquaresInBoard - 1 - static_cast<int>(s))));
		s = Square::A1;
		REQUIRE(reverse(fromSq(s)) == fromSq(static_cast<Square>(numSquaresInBoard - 1 - static_cast<int>(s))));

		REQUIRE(reverse(RankSecond) == RankFifth);
		REQUIRE(reverse(RankFirst) == RankSixth);

		REQUIRE(reverse(FileB) == FileE);
		REQUIRE(reverse(FileA) == FileF);
	}
}