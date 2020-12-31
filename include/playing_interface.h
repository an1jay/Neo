#include "position.h"

#include <random>

#pragma once

struct Player
{
      public:
	virtual ~Player() = default;
	virtual void updatePosition(Ply p) = 0;
	virtual Ply getNextMove() final
	{
		Ply p = _getPly();
		updatePosition(p);
		return p;
	}

      private:
	virtual Ply _getPly() = 0;
};

struct Game
{
      public:
	Game(std::function<Player*()> wht, std::function<Player*()> blk);
	~Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	bool playPlyList(std::vector<std::string> plies, bool verbose, bool printEndOnly);
	GameResult play(bool verbose = true);

      private:
	std::vector<Ply> _pliesList;
	Position _pos;
	Player* _wht;
	Player* _blk;
};

struct ConsolePlayer : Player
{
      public:
	ConsolePlayer() = default;
	~ConsolePlayer() = default;
	void updatePosition(Ply p) override { _pos.doPly(p); }

      private:
	Ply _getPly() override;
	Position _pos;
};

struct RandomPlayer : Player
{
      public:
	RandomPlayer(int seed, bool verbose)
	  : _seed(seed)
	  , _verbose(verbose)
	{}
	~RandomPlayer() = default;
	void updatePosition(Ply p) override { _pos.doPly(p); }

      private:
	Ply _getPly() override;
	Position _pos;
	int _seed;
	bool _verbose;
};

struct ClassicalPlayer : Player
{
      public:
	ClassicalPlayer(int depth, std::function<Score(Position&)> evaluator, bool _verbose);
	~ClassicalPlayer() = default;
	void updatePosition(Ply p) override { _pos.doPly(p); }

      private:
	Position _pos;
	std::function<Score(Position&)> _eval;
	int _depth;
	bool _verbose;

	Ply _getPly() override;
	Score _search(int depth, Color sideToPlay, int& nodeCount);
};