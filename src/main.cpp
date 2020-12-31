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
	auto consolePlayerCreater = []() { return new ConsolePlayer(); };

	auto minimaxPlayerCreaterCreater =
	  [](int depth, std::function<Score(Position&)> evaluator, bool _verbose) {
		  return [depth, evaluator, _verbose]() {
			  return new ClassicalPlayer(depth, evaluator, _verbose);
			  //   return new ClassicalPlayer(5, Eval::EvalFunc, true);
		  };
	  };

	Game g(minimaxPlayerCreaterCreater(7, Eval::EvalFunc, true), consolePlayerCreater);

	std::vector<std::string> plyStrs = {
		"F2F3", "C5C4", // 1
		"F3F4", "E5F4", // 2
		"B2B3", "C6C5", // 3
		"C1A3", "D5D4", // 4
		"B3C4", "B6C4", // 5
		"A3C5", "D6C5", // 6
		"C2C3", "C5D5", // 7
		"C3D4", "B5B4", // 8
		"A2A3", "E6D4", // 9
		"A3B4", "A5B4", // 10
		"A1A6", "F6A6", // 11
		"E1D3", "A6A1", // 12
		"D3B4", "D5D6", // 13
		"B4C2", "A1B1", // 14
	};

	// g.playPlyList(plyStrs, true, true);

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