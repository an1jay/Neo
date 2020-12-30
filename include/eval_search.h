#include "playing_interface.h"

#pragma once

struct ClassicalPlayer : Player
{
      public:
	ClassicalPlayer(int depth, std::function<Score(Position&)> evaluator, bool _verbose);
	~ClassicalPlayer() = default;
	void updatePosition(Ply p) override { _pos.doPly(p); }

      private:
	Position _pos;
	std::function<Score(Position&)> _eval;
	int _depth;
	bool _verbose;

	Ply _getPly() override;
	Score _search(int depth, Color sideToPlay, int& nodeCount);
};

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