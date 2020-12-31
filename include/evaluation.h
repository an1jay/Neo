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

constexpr Score WhiteWinScore = 10000;
constexpr Score BlackWinScore = -10000;

Score
EvalFunc(Position& pos);

} // namespace Eval