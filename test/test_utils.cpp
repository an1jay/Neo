#include "catch.h"

#include "constants.h"
#include "magics.h"
#include "utils.h"

TEST_CASE("Square File Rank", "[SqFileRank]")
{
	SECTION("generate Square from File and Rank")
	{
		REQUIRE(sqFromFileRank(File::A, Rank::Sixth) == Square::A6);
		REQUIRE(sqFromFileRank(File::D, Rank::Fifth) == Square::D5);
		REQUIRE(sqFromFileRank(File::F, Rank::First) == Square::F1);
		REQUIRE(sqFromFileRank(File::E, Rank::Second) == Square::E2);
	}

	SECTION("get File from Square")
	{
		REQUIRE(fileFromSq(Square::A5) == File::A);
		REQUIRE(fileFromSq(Square::B5) == File::B);
		REQUIRE(fileFromSq(Square::C5) == File::C);
		REQUIRE(fileFromSq(Square::F5) == File::F);
	}

	SECTION("get Rank from Square")
	{
		REQUIRE(rankFromSq(Square::A5) == Rank::Fifth);
		REQUIRE(rankFromSq(Square::B4) == Rank::Fourth);
		REQUIRE(rankFromSq(Square::C1) == Rank::First);
		REQUIRE(rankFromSq(Square::F6) == Rank::Sixth);
	}
}

TEST_CASE("Constants", "[Constants]")
{
	SECTION("Bishop")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::Bishop[sq] == HQBishopAttack(static_cast<Square>(sq), NoSquares));
			REQUIRE(AttackVectors::Bishop[sq] == ((AttackVectors::Diagonals[sq] | AttackVectors::AntiDiagonals[sq]) & ~fromSq(static_cast<Square>(sq))));
		}
	}

	SECTION("Rook")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::Rook[sq] == HQRookAttack(static_cast<Square>(sq), NoSquares));
		}
	}

	SECTION("Queen")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::Queen[sq] == HQQueenAttack(static_cast<Square>(sq), NoSquares));
		}
	}

	SECTION("BishopNoEdges")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::BishopMagicsOccupancyMask[sq] == genBishopMagicOccupancyMask(static_cast<Square>(sq)));
		}
	}

	SECTION("RookNoEdges")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::RookMagicsOccupancyMask[sq] == genRookMagicOccupancyMask(static_cast<Square>(sq)));
		}
	}
}