#include "position.h"

#pragma once

struct Player
{
      public:
	virtual ~Player() = default;
	virtual Ply getNextMove() = 0;
	virtual void updatePosition(Ply p) = 0;
};

struct Game
{
      public:
	Game(std::function<Player*()> wht, std::function<Player*()> blk);
	~Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	GameResult play(bool verbose = true);

      private:
	Position _pos;
	Player* _wht;
	Player* _blk;
};

struct ConsolePlayer : Player
{
      public:
	ConsolePlayer() = default;
	~ConsolePlayer() = default;
	Ply getNextMove() override;
	void updatePosition(Ply p) override
	{
		_pos.doPly(p);
		std::cout << "Doing ply " << p << std::endl;
	}

      private:
	Position _pos;
	Ply parsePly(std::string ply);
	// Below functions return false in second element if invalid inputs
	std::pair<Square, bool> parseSq(std::string s);
	std::pair<PieceType, bool> parsePieceType(char s);
};
