#include "search.h"

#include "evaluation.h"
#include "playing_interface.h"
#include "position.h"
#include "types.h"

#include <cassert>
#include <vector>

Score
ClassicalPlayer::_search(const int depth, Color sideToPlay, int& nodeCount)
{
	if (depth == 0 || _pos.currentGameResult() != GameResult::NB_NONE) {
		return _eval(_pos);
	}
	assert(sideToPlay != Color::NB_NONE);

	Score value = Eval::evalCoeff(sideToPlay) * Eval::BlackWinScore;
	std::vector<Ply> legalPlies = _pos.generateLegalPlies();

	for (Ply p : legalPlies) {
		_pos.doPly(p);
		nodeCount++;
		if (sideToPlay == Color::White)
			value = std::max(value, _search(depth - 1, Color::Black, nodeCount));
		else
			value = std::min(value, _search(depth - 1, Color::White, nodeCount));
		_pos.undoPly(p);
	}
	return value;
}