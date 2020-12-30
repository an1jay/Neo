#include "eval_search.h"

#include "position.h"

#include <chrono>

namespace Eval {

Score
EvalFunc(Position& pos)
{
	GameResult gr = pos.currentGameResult();
	Score eval = 0;
	if (gr == GameResult::NB_NONE) {
		// for now material only
		for (int i = 0; i < static_cast<int>(Piece::NB_NONE); i++) {
			eval += Eval::materialWeights[i] * pos.pieceCount(static_cast<Piece>(i));
		}
	} else {
		if (gr == GameResult::BlackWin)
			return Eval::BlackWinScore;
		if (gr == GameResult::WhiteWin)
			return Eval::WhiteWinScore;
		if (gr == GameResult::Draw)
			return 0;
	}
	return eval;
}

} // namespace Eval

ClassicalPlayer::ClassicalPlayer(int depth, std::function<Score(Position&)> evaluator, bool verbose)
{
	_depth = depth;
	_eval = evaluator;
	_verbose = verbose;
}

Ply
ClassicalPlayer::_getPly()
{

	std::cout << "Classical player thinks..." << std::endl;

	std::vector<Ply> legalPlies = _pos.generateLegalPlies();
	std::vector<Score> plyScores;
	plyScores.reserve(legalPlies.size());
	Color sideToMove = _pos.sideToMove();

	Score currentScore = 0;
	Score bestScore = Eval::evalCoeff(sideToMove) * Eval::BlackWinScore;

	Ply bestPly = INVALID_PLY;

	int nodeCount = 0;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (Ply p : legalPlies) {
		_pos.doPly(p);
		nodeCount++;
		// std::cout << "_getPly iterations - nodecount - " << nodeCount << std::endl;
		// std::cout << _pos << asPlyString(p) << std::endl << "+++++++++++++++" << std::endl;
		currentScore = _search(_depth, otherColor(sideToMove), nodeCount);
		_pos.undoPly(p);
		plyScores.push_back(currentScore);

		if (sideToMove == Color::White) {
			if (currentScore > bestScore) {

				bestScore = currentScore;
				bestPly = p;
			}
		} else {
			if (currentScore < bestScore) {

				bestScore = currentScore;
				bestPly = p;
			}
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	if (_verbose) {
		std::cout << _pos << std::endl << "Move Evaluations " << std::endl;
		for (int i = 0; i < static_cast<int>(legalPlies.size()); i++) {
			std::cout << asPaddedPlyString(legalPlies[i], 10) << plyScores[i]
				  << std::endl;
		}

		double timeDelta =
		  (static_cast<double>(
		     std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) /
		   1000000000.0);

		std::cout.imbue(std::locale(""));
		std::cout << std::endl
			  << "Chosen Move " << asPlyString(bestPly) << " | Score " << bestScore
			  << " | Time taken " << timeDelta << "[s] | Nodes " << nodeCount
			  << " | Nodes/s "
			  << static_cast<double>(nodeCount) / static_cast<double>(timeDelta)
			  << std::endl;
	}
	return bestPly;
}

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
