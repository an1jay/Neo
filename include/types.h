#include <cstdint>

#pragma once

// ***** Types *****

// BitBoard encodes as following:
// 0000000000000000000000000000 | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1
// Squares                      | F6 E6 D6 C6 B6 A6 | F5 E5 D5 C5 B5 A5 | F4 E4 D4 C4 B4 A4 | F3 E3 D3 C3 B3 A3 | F2 E2 D2 C2 B2 A2 | F1 E1 D1 C1 B1 A1
// Numbers                      | 35 34 33 32 31 30 | 29 28 27 26 25 24 | 23 22 21 20 19 18 | 17 16 15 14 13 12 | 11 10 9  8  7  6  | 5  4  3  2  1  0
using BitBoard = uint64_t;

enum class Color : uint8_t
{
	Black,
	White,
	NONE,
};

// TODO: might be more useful to have Piece - e.g. WPawn, WKnight, etc.
enum class PieceType : uint8_t
{
	Pawn,
	Knight,
	Rook,
	Queen,
	King,
	NONE,
};

enum class Square : uint8_t
{
	// clang-format off
	A1, A2, A3, A4, A5, A6,
	B1, B2, B3, B4, B5, B6,
	C1, C2, C3, C4, C5, C6,
	D1, D2, D3, D4, D5, D6,
	E1, E2, E3, E4, E5, E6,
	F1, F2, F3, F4, F5, F6,
	NONE,
	// clang-format on
};

enum class File : uint8_t
{
	// clang-format off
	A, B, C, D, E, F,
	NONE,
	// clang-format on
};

enum class Rank : uint8_t
{
	// clang-format off
	First, Second, Third, Fourth, Fifth, Sixth,
	NONE,
	// clang-format on
};