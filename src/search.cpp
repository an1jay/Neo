#include "search.h"

#include "evaluation.h"
#include "playing_interface.h"
#include "position.h"
#include "types.h"

#include <cassert>
#include <vector>

Score
ClassicalPlayer::_search(const int depth,
			 Color sideToPlay,
			 int& nodeCount,
			 Score alpha,
			 Score beta,
			 int pliesFromRoot)
{
	if (depth == 0) {
		return _eval(_pos);
	}

	const GameResult currentGR = _pos.currentGameResult();
	if (currentGR != GameResult::NB_NONE) {
		if (currentGR == GameResult::WhiteWin)
			return Eval::WhiteWinScore - pliesFromRoot;
		if (currentGR == GameResult::BlackWin)
			return Eval::BlackWinScore + pliesFromRoot;
		if (currentGR == GameResult::Draw)
			return 0;
	}

	assert(sideToPlay != Color::NB_NONE);

	Score value = Eval::evalCoeff(sideToPlay) * Eval::BlackWinScore;
	std::vector<Ply> legalPlies = _pos.generateLegalPlies();

	for (Ply p : legalPlies) {
		_pos.doPly(p);
		nodeCount++;
		if (sideToPlay == Color::White) {
			value = std::max(
			  value,
			  _search(
			    depth - 1, Color::Black, nodeCount, alpha, beta, pliesFromRoot + 1));
			alpha = std::max(alpha, value);
			if (alpha >= beta) {
				_pos.undoPly(p);
				break;
			}
		} else {
			value = std::min(
			  value,
			  _search(
			    depth - 1, Color::White, nodeCount, alpha, beta, pliesFromRoot + 1));
			beta = std::min(beta, value);
			if (beta <= alpha) {
				_pos.undoPly(p);
				break;
			}
		}
		_pos.undoPly(p);
	}
	return value;
}