#include "playing_interface.h"

#include "evaluation.h"
#include "move_gen.h"
#include "terminal_colors.h"
#include "utils.h"

#include <chrono>
#include <iomanip>

Game::Game(std::function<Player*()> wht, std::function<Player*()> blk)
{
	_wht = wht();
	_blk = blk();
}

Game::~Game()
{
	delete _wht;
	delete _blk;
}

bool
Game::playPlyList(std::vector<std::string> plyStrs, bool verbose, bool printEndOnly)
{

	std::vector<Ply> plies;
	Ply p;

	for (std::string plyStr : plyStrs) {
		p = parsePly(plyStr);
		if (p == INVALID_PLY)
			return false;
		plies.push_back(p);
	}

	for (Ply validPly : plies) {

		if (!printEndOnly && verbose)
			std::cout << _pos.sideToMove() << " played " << asPlyString(validPly)
				  << std::endl;

		_wht->updatePosition(validPly);
		_blk->updatePosition(validPly);
		_pos.doPly(validPly);
		if (!printEndOnly && verbose) {
			std::cout << _pos;
			std::cout << std::string(25, '*') << std::endl;
		}
	}

	std::cout << "Position at the end: " << std::endl << _pos << "Played Moves: " << std::endl;
	printMoveHistory(plies);
	std::cout << std::endl;
	return true;
}

GameResult
Game::play(bool verbose)
{
	GameResult res = GameResult::NB_NONE;

	Color sideToMove;
	Player* playerToMove;
	Ply currentMove;

	Ply previousMove = INVALID_PLY;

	// While game is in play
	//  - send last played move to the player to move (except on first loop)
	//  - request move from player
	//  - make move on internal position
	//  - check game result on position

	while (res == GameResult::NB_NONE) {
		// Get side & player to move
		sideToMove = _pos.sideToMove();
		assert(sideToMove != Color::NB_NONE);
		playerToMove = sideToMove == Color::White ? _wht : _blk;

		// update player on last move made
		if (previousMove != INVALID_PLY)
			playerToMove->updatePosition(previousMove);

		// get move from player to move; make the move; set previous move to current move
		currentMove = playerToMove->getNextMove();
		_pos.doPly(currentMove);
		_pliesList.push_back(currentMove);
		previousMove = currentMove;

		// update game information
		res = _pos.currentGameResult();
	}

	if (verbose) {
		std::cout << "Game finished in " << _pos.currentPlyClock() << " plies." << std::endl
			  << "Result: " << res << std::endl
			  << std::endl
			  << "Move List: " << std::endl;

		printMoveHistory(_pliesList);
	}

	return res;
}

Ply
ConsolePlayer::_getPly()
{
	std::vector<Ply> legalPlies = _pos.generateLegalPlies();
	std::cout << _pos;
	std::cout << "Legal Moves: " << std::endl;
	printPlyList(legalPlies);

	bool validPly = true;
	std::string plyStr;
	Ply p;
	do {
		std::cout << "Enter ply: ";
		std::cin >> plyStr;
		p = parsePly(plyStr);
		validPly = plyInList(p, legalPlies);
		if (!validPly)
			std::cout << std::endl << "Invalid ply. Try again! ";
	} while (!validPly);
	return p;
}

Ply
RandomPlayer::_getPly()
{

	std::vector<Ply> legalPlies = _pos.generateLegalPlies();
	srand(_seed);
	int moveIndex = rand() % legalPlies.size();
	if (_verbose) {
		std::cout << _pos;
		std::cout << "Legal Moves: " << std::endl;
		printPlyList(legalPlies);
		std::cout << "Chosen Move " << asPlyString(legalPlies[moveIndex]) << std::endl;
	}
	return legalPlies[moveIndex];
}

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
