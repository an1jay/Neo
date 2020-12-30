#include "playing_interface.h"

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

		for (int i = 0; i < static_cast<int>(_pliesList.size()); i += 2) {
			std::cout << std::left << std::setfill(' ') << std::setw(3) << (i / 2) + 1
				  << asPaddedPlyString(_pliesList[i], 10) << "|"
				  << asPaddedPlyString(_pliesList[i + 1], 10) << std::endl;
		}
	}

	return res;
}

Ply
ConsolePlayer::_getPly()
{
	std::vector<Ply> legalPlies = _pos.generateLegalPlies();
	std::cout << _pos;
	std::cout << "Legal Moves: " << std::endl;
	printMoveList(legalPlies);

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
ConsolePlayer::parsePly(std::string ply)
{
	if (!(ply.length() == 4 || ply.length() == 6))
		return INVALID_PLY;

	std::pair<Square, bool> origin = parseSq(ply.substr(0, 2));
	std::pair<Square, bool> dest = parseSq(ply.substr(2, 2));
	std::pair<PieceType, bool> promotion = { PieceType::NB_NONE, true };

	if (ply.length() == 6) {
		promotion = parsePieceType(ply[5]);
	}

	if (!origin.second || !dest.second || !promotion.second)
		return INVALID_PLY;

	return encodePly(dest.first, origin.first, promotion.first);
}

std::pair<Square, bool>
ConsolePlayer::parseSq(std::string s)
{

	bool valid =
	  (s.length() == 2) && (s[0] >= 'A' && s[0] <= 'F') && (s[1] >= '1' && s[1] <= '6');
	if (!valid)
		return { Square::NB_NONE, true };

	return { sqFromFileRank(static_cast<File>(s[0] - 'A'), static_cast<Rank>(s[1] - '1')),
		 true };
}

std::pair<PieceType, bool>
ConsolePlayer::parsePieceType(char s)
{
	if (s == 'Q')
		return { PieceType::Queen, true };

	if (s == 'R')
		return { PieceType::Rook, true };

	if (s == 'N')
		return { PieceType::Knight, true };

	return { PieceType::NB_NONE, false };
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
		printMoveList(legalPlies);
		std::cout << "Chosen Move " << asPlyString(legalPlies[moveIndex]) << std::endl;
	}
	return legalPlies[moveIndex];
}