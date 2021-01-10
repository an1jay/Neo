#include "constants.h"
#include "types.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>

#pragma once

template<typename T>
T
genRand(std::mt19937& rng)
{
	const uint64_t bottomTwoBytes = 0xFFFFULL;
	return static_cast<T>((static_cast<uint64_t>(rng()) & bottomTwoBytes) |
			      ((static_cast<uint64_t>(rng()) & bottomTwoBytes) << 16) |
			      ((static_cast<uint64_t>(rng()) & bottomTwoBytes) << 32) |
			      ((static_cast<uint64_t>(rng()) & bottomTwoBytes) << 48));
}

template<typename T>
std::string
formatNumber(T num, int precision = 2, int padding = 0, bool commaThousands = true)
{
	std::ostringstream numStr;
	if (num < 0) {
		numStr << '-';
		num = -num;
	}

	long int intBody = static_cast<long int>(num);
	long int decimal;

	const std::string number = std::to_string(intBody);
	int paddingConstant;
	if (commaThousands) {
		for (int i = 0; i < static_cast<int>(number.size()); i++) {
			paddingConstant = static_cast<int>(number.size()) - (i);
			if (paddingConstant % 3 == 0 && paddingConstant != 0 && i != 0)
				numStr << ',';
			numStr << number[i];
		}
	} else {
		numStr << num;
	}

	std::is_floating_point<T> isFloating;

	if (isFloating && precision != 0) {
		decimal = static_cast<long int>(fmod(num, 1) * pow(10, precision));
		if (decimal == 0) {
			numStr << '.' << std::string(precision, '0');
		} else
			numStr << '.' << decimal;
	}

	if (padding != 0) {
		std::string unpadded = numStr.str();
		if (padding > static_cast<int>(unpadded.size())) {
			numStr.str("");
			numStr << std::string(padding - static_cast<int>(unpadded.size()), ' ')
			       << unpadded;
		}
	}

	return numStr.str();
}

bool
plyInList(Ply p, std::vector<Ply> plyList);

void
printPlyList(std::vector<Ply> pList);

void
printMoveHistory(std::vector<Ply> pList);

std::string boardPrint(std::function<std::pair<char, Color>(Square)>);

std::ostream&
operator<<(std::ostream& os, Square s);

std::ostream&
operator<<(std::ostream& os, Color c);

std::ostream&
operator<<(std::ostream& os, Piece p);

std::ostream&
operator<<(std::ostream& os, GameResult gr);

std::ostream&
operator<<(std::ostream& os, PieceType pt);

std::string
asBoardString(const BitBoard b);

std::string
asPlyString(const Ply p);

std::string
asPaddedPlyString(const Ply p, const int width);

// Returns `INVALID_PLY` if invalid input
Ply
parsePly(std::string ply);

// Returns false in second element if invalid input
std::pair<Square, bool>
parseSq(std::string s);

// Returns false in second element if invalid input
std::pair<PieceType, bool>
parsePieceType(char s);

constexpr Square
sqFromFileRank(const File f, const Rank r)
{
	if (f == File::NB_NONE || r == Rank::NB_NONE)
		return Square::NB_NONE;
	else
		return static_cast<Square>(static_cast<int>(r) * numSquaresInRankFile +
					   static_cast<int>(f));
}

constexpr File
fileFromSq(const Square s)
{
	return static_cast<File>(static_cast<int>(s) % numSquaresInRankFile);
}

constexpr Rank
rankFromSq(const Square s)
{
	return static_cast<Rank>(static_cast<int>(s) / numSquaresInRankFile);
}

constexpr Square
sqFromBB(const BitBoard b)
{
	assert(b != NoSquares);
	return static_cast<Square>(__builtin_ctzll(b));
}

constexpr Piece
pieceFromPieceTypeColor(const PieceType pt, const Color c)
{
	if (pt == PieceType::NB_NONE || c == Color::NB_NONE)
		return Piece::NB_NONE;
	return static_cast<Piece>(static_cast<int>(pt) +
				  (static_cast<int>(c) * static_cast<int>(Piece::B_Pawn)));
}

constexpr PieceType
pieceTypeFromPiece(const Piece p)
{
	if (p == Piece::NB_NONE)
		return PieceType::NB_NONE;
	return static_cast<PieceType>(static_cast<int>(p) % static_cast<int>(Piece::B_Pawn));
}

constexpr Color
colorFromPiece(const Piece p)
{
	if (p == Piece::NB_NONE)
		return Color::NB_NONE;
	if (static_cast<int>(p) <= static_cast<int>(Piece::W_King))
		return Color::White;
	return Color::Black;
}

constexpr Color
otherColor(Color c)
{
	if (c == Color::Black)
		return Color::White;
	else if (c == Color::White)
		return Color::Black;
	return Color::NB_NONE;
}
