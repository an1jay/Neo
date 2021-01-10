#include "search.h"

#include "evaluation.h"
#include "playing_interface.h"
#include "position.h"
#include "types.h"

#include <cassert>
#include <chrono>
#include <vector>

ClassicalPlayer::ClassicalPlayer(int depth,
				 int maxDepth,
				 std::function<Score(Position&)> evaluator,
				 bool verbose)
{
	// assert(maxDepth >= depth);
	_depth = depth;
	_maxDepth = maxDepth;
	_eval = evaluator;
	_verbose = verbose;
}

Ply
ClassicalPlayer::_getPly()
{
	std::vector<Ply> legalPlies = _pos.generateLegalPlies().first;
	// TODO whether we should do something different with quiet / loud moves

	std::vector<Score> plyScores;
	plyScores.reserve(legalPlies.size());
	Color sideToMove = _pos.sideToMove();
	Score currentScore = 0;
	Score bestScore = Eval::evalCoeff(sideToMove) * Eval::MIN_SCORE;
	Ply bestPly = INVALID_PLY;
	int totalNodeCount = 0;
	int nodeCount = 0;

	const int pieceCount = _pos.pieceCount();
	int addtlDepth = (pieceCount < 7) ? 3 : ((pieceCount < 13) ? 1 : 0);
	int searchDepth = _depth + addtlDepth;

	if (_verbose)
		std::cout << "Classical player thinks (at depth [" << searchDepth << ", "
			  << _maxDepth + addtlDepth << "])..." << std::endl;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	if (legalPlies.size() == 1) {
		bestPly = legalPlies[0];
		bestScore = 0;
		if (_verbose)
			std::cout << "Move: " << asPlyString(bestPly)
				  << " | Score: [only move] | Nodes: [n/a]" << std::endl;
	} else
		for (Ply p : legalPlies) {
			_pos.doPly(p);

			if (_verbose)
				std::cout << "Searching Ply: " << asPlyString(p) << std::flush;

			nodeCount = 1;
			currentScore = _search(searchDepth,
					       _maxDepth + addtlDepth,
					       otherColor(sideToMove),
					       nodeCount,
					       Eval::MIN_SCORE,
					       Eval::MAX_SCORE,
					       1);
			_pos.undoPly(p);
			plyScores.push_back(currentScore);
			totalNodeCount += nodeCount;
			if (_verbose)
				std::cout << " | Score: " << formatNumber(currentScore, 2, 11)
					  << " | Nodes: " << formatNumber(nodeCount, 2, 11)
					  << " | Depth: [" << searchDepth << ", "
					  << _maxDepth + addtlDepth << "]" << std::endl;

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

		std::cout << "Chosen Move: " << asPlyString(bestPly)
			  << " | Score: " << formatNumber(bestScore, 2, 5)
			  << " | Time Taken: " << formatNumber(timeDelta, 3, 7)
			  << "[s] | Nodes: " << formatNumber(totalNodeCount, 2, 11)
			  << " | Nodes/s: "
			  << formatNumber(static_cast<double>(totalNodeCount) /
					    static_cast<double>(timeDelta),
					  0,
					  11)
			  << std::endl
			  << std::endl;
	}
	return bestPly;
}

Score
ClassicalPlayer::_search(const int depth,
			 const int maxDepth,
			 Color sideToPlay,
			 int& nodeCount,
			 Score alpha,
			 Score beta,
			 int pliesFromRoot)
{
	const GameResult currentGR = _pos.currentGameResult();
	if (currentGR != GameResult::NB_NONE) {
		if (currentGR == GameResult::WhiteWin)
			return Eval::WhiteWinScore - pliesFromRoot;
		if (currentGR == GameResult::BlackWin)
			return Eval::BlackWinScore + pliesFromRoot;
		if (currentGR == GameResult::Draw)
			return 0;
	}

	if (depth == 0) {
		return _eval(_pos);
	}

	assert(sideToPlay != Color::NB_NONE);

	Score value = Eval::evalCoeff(sideToPlay) * Eval::BlackWinScore;

	std::pair<std::vector<Ply>, int> legalPliesAndQuietMoveIndex = _pos.generateLegalPlies();
	std::vector<Ply> legalPlies = legalPliesAndQuietMoveIndex.first;
	const int quietMoveIndex = legalPliesAndQuietMoveIndex.second;

	int quiescenceAwareDepth;
	for (int i = 0; i < static_cast<int>(legalPlies.size()); i++) {
		_pos.doPly(legalPlies[i]);
		nodeCount++;

		// Search non quiet moves more
		quiescenceAwareDepth = (i < quietMoveIndex) && (pliesFromRoot < maxDepth)
					 ? std::max(depth - 1, 1)
					 : depth - 1;

		if (sideToPlay == Color::White) {
			value = std::max(value,
					 _search(quiescenceAwareDepth,
						 maxDepth,
						 Color::Black,
						 nodeCount,
						 alpha,
						 beta,
						 pliesFromRoot + 1));
			alpha = std::max(alpha, value);
			if (alpha >= beta) {
				_pos.undoPly(legalPlies[i]);
				break;
			}
		} else {
			value = std::min(value,
					 _search(quiescenceAwareDepth,
						 maxDepth,
						 Color::White,
						 nodeCount,
						 alpha,
						 beta,
						 pliesFromRoot + 1));
			beta = std::min(beta, value);
			if (beta <= alpha) {
				_pos.undoPly(legalPlies[i]);
				break;
			}
		}
		_pos.undoPly(legalPlies[i]);
	}
	return value;
}