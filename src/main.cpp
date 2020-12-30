#include "bitboard.h"
#include "constants.h"
#include "eval_search.h"
#include "magics.h"
#include "playing_interface.h"
#include "position.h"
#include "utils.h"

#include <iostream>

int
main()
{
	// printRookMagicOccupancyMask();
	// std::cout << asBoardString(AllSquares) << std::endl;

	auto consolePlayerCreater = []() { return new ConsolePlayer(); };

	auto minimaxPlayerCreaterCreater =
	  [](int depth, std::function<Score(Position&)> evaluator, bool _verbose) {
		  return [depth, evaluator, _verbose]() {
			  return new ClassicalPlayer(depth, evaluator, _verbose);
			  //   return new ClassicalPlayer(5, Eval::EvalFunc, true);
		  };
	  };

	Game g(minimaxPlayerCreaterCreater(5, Eval::EvalFunc, true), consolePlayerCreater);

	g.play();

	return 0;
}

void
demoPlay()
{
	Position p;

	for (int i = 0; i < 200; i++) {
		std::cout << i;
		std::cout << p;

		std::cout << "Pseudo" << std::endl;
		auto pseudomoveList = p.generatePseudoLegalPlies();
		for (auto m : pseudomoveList)
			std::cout << asPlyString(m) << " | ";
		std::cout << std::endl;

		std::cout << "Legal" << std::endl;
		auto moveList = p.generateLegalPlies();
		for (auto m : moveList)
			std::cout << asPlyString(m) << " | ";

		std::cout << std::endl;
		p.doPly(moveList[0]);
	}
}

void
demoRandomPlayer()
{
	auto randomPlayerCreaterCreater = [](int s) {
		return [s]() { return new RandomPlayer(s, false); };
	};

	Game g(randomPlayerCreaterCreater(1), randomPlayerCreaterCreater(2));

	g.play();
}