#include <cstdint>

#pragma once

// ***** Types *****

// clang-format off
// BitBoard encodes as following:
// 0000000000000000000000000000 | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1  | 1  1  1  1  1  1
// Squares                      | F6 E6 D6 C6 B6 A6 | F5 E5 D5 C5 B5 A5 | F4 E4 D4 C4 B4 A4 | F3 E3 D3 C3 B3 A3 | F2 E2 D2 C2 B2 A2 | F1 E1 D1 C1 B1 A1
// Numbers                      | 35 34 33 32 31 30 | 29 28 27 26 25 24 | 23 22 21 20 19 18 | 17 16 15 14 13 12 | 11 10 9  8  7  6  | 5  4  3  2  1  0
// clang-format on
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
	A1, B1, C1, D1, E1, F1,
	A2, B2, C2, D2, E2, F2,
	A3, B3, C3, D3, E3, F3,
	A4, B4, C4, D4, E4, F4,
	A5, B5, C5, D5, E5, F5,
	A6, B6, C6, D6, E6, F6,
	// clang-format on
	NONE,
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