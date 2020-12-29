#include "playing_interface.h"

#include "move_gen.h"

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

		std::cout << "Game::play - sideToMove= " << sideToMove << std::endl;
		std::cout << "Game::play - halfmoveclock= " << _pos.currentPlyClock() << std::endl;

		// update player on last move made
		if (previousMove != INVALID_PLY)
			playerToMove->updatePosition(previousMove);

		// get move from player to move; make the move; set previous move to current move
		currentMove = playerToMove->getNextMove();
		_pos.doPly(currentMove);
		previousMove = currentMove;

		// update game information
		res = _pos.currentGameResult();
	}

	if (verbose) {
		std::cout << "Game finished in " << _pos.currentPlyClock() << " plies." << std::endl
			  << "Result: " << res << std::endl;
	}

	return res;
}

Ply
ConsolePlayer::getNextMove()
{
	std::vector<Ply> legalPlies = _pos.generateLegalPlies();
	std::cout << _pos;
	for (int i = 0; i < static_cast<int>(legalPlies.size()); i++) {
		std::cout << asPlyString(legalPlies[i]);
		if (i < (static_cast<int>(legalPlies.size()) - 1))
			std::cout << " | ";
	}
	std::cout << std::endl;

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

	_pos.doPly(p);

	return p;
}

Ply
ConsolePlayer::parsePly(std::string ply)
{
	assert(ply.length() == 4 || ply.length() == 6);

	// std::cout << "Ply len" << ply.length() << std::endl;

	std::pair<Square, bool> origin = parseSq(ply.substr(0, 2));
	std::pair<Square, bool> dest = parseSq(ply.substr(2, 2));
	std::pair<PieceType, bool> promotion = { PieceType::NB_NONE, true };

	// std::cout << "origin " << origin.first << " | " << origin.second << std::endl;
	// std::cout << "dest " << dest.first << " | " << dest.second << std::endl;
	// std::cout << "promotion " << promotion.first << " | " << promotion.second << std::endl;

	if (ply.length() == 6) {
		// std::cout << "Ply length = 6" << std::endl;
		promotion = parsePieceType(ply[5]);
	}

	if (!origin.second || !dest.second || !promotion.second)
		return INVALID_PLY;

	return encodePly(dest.first, origin.first, promotion.first);
}

std::pair<Square, bool>
ConsolePlayer::parseSq(std::string s)
{

	// std::cout << "parse Sq: s=" << s << std::endl;

	bool valid =
	  (s.length() == 2) && (s[0] >= 'A' && s[0] <= 'F') && (s[1] >= '1' && s[1] <= '6');
	if (!valid)
		return { Square::NB_NONE, true };

	// std::cout << "static_cast<File>(s[0] - 'A') " << (s[0] - 'A') << std::endl;
	// std::cout << "static_cast<File>(s[1] - '1') " << (s[1] - '1') << std::endl;
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
