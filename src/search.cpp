#include "search.h"

#include "evaluation.h"
#include "playing_interface.h"
#include "position.h"
#include "types.h"

#include <cassert>
#include <chrono>
#include <vector>

ClassicalPlayer::ClassicalPlayer(int depth, std::function<Score(Position&)> evaluator, bool verbose)
{
	_depth = depth;
	_eval = evaluator;
	_verbose = verbose;
}

Ply
ClassicalPlayer::_getPly()
{

	std::vector<Ply> legalPlies = _pos.generateLegalPlies();
	std::vector<Score> plyScores;
	plyScores.reserve(legalPlies.size());
	Color sideToMove = _pos.sideToMove();
	Score currentScore = 0;
	Score bestScore = Eval::evalCoeff(sideToMove) * Eval::MIN_SCORE;
	Ply bestPly = INVALID_PLY;
	int nodeCount = 0;

	const int pieceCount = _pos.pieceCount();
	int searchDepth = (pieceCount < 7) ? _depth + 3 : ((pieceCount < 13) ? _depth + 1 : _depth);

	if (_verbose)
		std::cout << "Classical player thinks (at depth " << searchDepth << ")..."
			  << std::endl;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::cout.imbue(std::locale("en_US.UTF8"));
	if (legalPlies.size() == 1) {
		bestPly = legalPlies[0];
		bestScore = 0;
		if (_verbose)
			std::cout << "Move: " << asPlyString(bestPly)
				  << " | Score: [only move] | Cum. Nodes: [n/a]" << std::endl;
	} else
		for (Ply p : legalPlies) {
			_pos.doPly(p);
			nodeCount++;
			currentScore = _search(searchDepth,
					       otherColor(sideToMove),
					       nodeCount,
					       Eval::MIN_SCORE,
					       Eval::MAX_SCORE,
					       1);
			_pos.undoPly(p);
			plyScores.push_back(currentScore);

			if (_verbose)
				std::cout
				  << "Move: " << asPlyString(p) << " | Score: " << currentScore
				  << " | Cum. Nodes: " << nodeCount << " | [depth " << searchDepth
				  << "]" << std::endl;

			if (sideToMove == Color::White) {
				// If found a mate already, reduce depth for later searches
				if (currentScore > Eval::WhiteWinScore - Eval::WIN_PLY_THRESHOLD)
					searchDepth = Eval::WhiteWinScore - currentScore;
				if (currentScore > bestScore) {
					bestScore = currentScore;
					bestPly = p;
				}
			} else {
				// If found a mate already, reduce depth for later searches
				if (currentScore < Eval::BlackWinScore - Eval::WIN_PLY_THRESHOLD)
					searchDepth = currentScore - Eval::BlackWinScore;
				if (currentScore < bestScore) {
					bestScore = currentScore;
					bestPly = p;
				}
			}
		}
	assert(bestPly != INVALID_PLY);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	if (_verbose) {
		std::cout << _pos;

		double timeDelta =
		  (static_cast<double>(
		     std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) /
		   1000000000.0);

		std::cout.imbue(std::locale("en_US.UTF8"));
		std::cout << "Chosen Move " << asPlyString(bestPly) << " | Score " << bestScore
			  << " | Time taken " << timeDelta << "[s] | Nodes " << nodeCount
			  << " | Nodes/s "
			  << static_cast<double>(nodeCount) / static_cast<double>(timeDelta)
			  << std::endl
			  << std::endl;
	}
	return bestPly;
}

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