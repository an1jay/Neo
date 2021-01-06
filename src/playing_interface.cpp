#include "playing_interface.h"

#include "evaluation.h"
#include "move_gen.h"
#include "terminal_colors.h"
#include "utils.h"

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
		_pliesList.push_back(validPly);
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
			  << "Position at end: " << _pos << std::endl
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
