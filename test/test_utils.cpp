#include "catch.h"
// include catch first
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

TEST_CASE("PieceType Color", "[PieceTypeColor]")
{
	SECTION("get Piece from PieceType Color")
	{
		REQUIRE(pieceFromPieceTypeColor(PieceType::Pawn, Color::Black) == Piece::B_Pawn);
		REQUIRE(pieceFromPieceTypeColor(PieceType::Pawn, Color::White) == Piece::W_Pawn);

		REQUIRE(pieceFromPieceTypeColor(PieceType::Knight, Color::Black) ==
			Piece::B_Knight);
		REQUIRE(pieceFromPieceTypeColor(PieceType::Knight, Color::White) ==
			Piece::W_Knight);

		REQUIRE(pieceFromPieceTypeColor(PieceType::Rook, Color::Black) == Piece::B_Rook);
		REQUIRE(pieceFromPieceTypeColor(PieceType::Rook, Color::White) == Piece::W_Rook);

		REQUIRE(pieceFromPieceTypeColor(PieceType::Queen, Color::Black) == Piece::B_Queen);
		REQUIRE(pieceFromPieceTypeColor(PieceType::Queen, Color::White) == Piece::W_Queen);

		REQUIRE(pieceFromPieceTypeColor(PieceType::King, Color::Black) == Piece::B_King);
		REQUIRE(pieceFromPieceTypeColor(PieceType::King, Color::White) == Piece::W_King);

		REQUIRE(pieceFromPieceTypeColor(PieceType::Knight, Color::NB_NONE) ==
			Piece::NB_NONE);
		REQUIRE(pieceFromPieceTypeColor(PieceType::NB_NONE, Color::White) ==
			Piece::NB_NONE);
	}

	SECTION("get PieceType from Piece")
	{
		REQUIRE(pieceTypeFromPiece(Piece::W_Pawn) == PieceType::Pawn);
		REQUIRE(pieceTypeFromPiece(Piece::W_Knight) == PieceType::Knight);
		REQUIRE(pieceTypeFromPiece(Piece::W_Rook) == PieceType::Rook);
		REQUIRE(pieceTypeFromPiece(Piece::W_Queen) == PieceType::Queen);
		REQUIRE(pieceTypeFromPiece(Piece::W_King) == PieceType::King);

		REQUIRE(pieceTypeFromPiece(Piece::B_Pawn) == PieceType::Pawn);
		REQUIRE(pieceTypeFromPiece(Piece::B_Knight) == PieceType::Knight);
		REQUIRE(pieceTypeFromPiece(Piece::B_Rook) == PieceType::Rook);
		REQUIRE(pieceTypeFromPiece(Piece::B_Queen) == PieceType::Queen);
		REQUIRE(pieceTypeFromPiece(Piece::B_King) == PieceType::King);

		REQUIRE(pieceTypeFromPiece(Piece::NB_NONE) == PieceType::NB_NONE);
	}

	SECTION("get Color from Piece")
	{
		REQUIRE(colorFromPiece(Piece::W_Pawn) == Color::White);
		REQUIRE(colorFromPiece(Piece::W_Knight) == Color::White);
		REQUIRE(colorFromPiece(Piece::W_Rook) == Color::White);
		REQUIRE(colorFromPiece(Piece::W_Queen) == Color::White);
		REQUIRE(colorFromPiece(Piece::W_King) == Color::White);

		REQUIRE(colorFromPiece(Piece::B_Pawn) == Color::Black);
		REQUIRE(colorFromPiece(Piece::B_Knight) == Color::Black);
		REQUIRE(colorFromPiece(Piece::B_Rook) == Color::Black);
		REQUIRE(colorFromPiece(Piece::B_Queen) == Color::Black);
		REQUIRE(colorFromPiece(Piece::B_King) == Color::Black);

		REQUIRE(colorFromPiece(Piece::NB_NONE) == Color::NB_NONE);
	}
}

TEST_CASE("Constants", "[Constants]")
{
	SECTION("Bishop")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::Bishop[sq] ==
				HQBishopAttack(static_cast<Square>(sq), NoSquares));
			REQUIRE(AttackVectors::Bishop[sq] ==
				((AttackVectors::Diagonals[sq] | AttackVectors::AntiDiagonals[sq]) &
				 ~BBfromSq(static_cast<Square>(sq))));
		}
	}

	SECTION("Rook")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::Rook[sq] ==
				HQRookAttack(static_cast<Square>(sq), NoSquares));
		}
	}

	SECTION("Queen")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::Queen[sq] ==
				HQQueenAttack(static_cast<Square>(sq), NoSquares));
		}
	}

	SECTION("BishopNoEdges")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::BishopMagicsOccupancyMask[sq] ==
				genBishopMagicOccupancyMask(static_cast<Square>(sq)));
		}
	}

	SECTION("RookNoEdges")
	{
		for (int sq = 0; sq < numSquaresInBoard; sq++) {
			REQUIRE(AttackVectors::RookMagicsOccupancyMask[sq] ==
				genRookMagicOccupancyMask(static_cast<Square>(sq)));
		}
	}
}