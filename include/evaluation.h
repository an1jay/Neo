#include "position.h"
#include "types.h"

#include <cassert>

#pragma once

namespace Eval {

inline Score
evalCoeff(Color c)
{
	assert(c != Color::NB_NONE);
	return c == Color::White ? 1 : -1;
}

constexpr int materialWeights[] = { 1, 2, 4, 7, 0, -1, -2, -4, -7, 0 };

constexpr Score WhiteWinScore = 1e4;
constexpr Score BlackWinScore = -1e4;

constexpr Score MAX_SCORE = WhiteWinScore + 1;
constexpr Score MIN_SCORE = BlackWinScore - 1;

Score
EvalFunc(Position& pos);

} // namespace Eval