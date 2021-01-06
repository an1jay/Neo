#include "bitboard.h"
#include "constants.h"
#include "evaluation.h"
#include "magics.h"
#include "playing_interface.h"
#include "position.h"
#include "utils.h"

#include <algorithm>
#include <iostream>

int
main()
{
	// auto consolePlayerCreater = []() { return []() { return new ConsolePlayer(); }; };

	auto minimaxPlayerCreaterCreater =
	  [](int depth, std::function<Score(Position&)> evaluator, bool _verbose) {
		  return [depth, evaluator, _verbose]() {
			  return new ClassicalPlayer(depth, evaluator, _verbose);
		  };
	  };

	// Game g(minimaxPlayerCreaterCreater(9, Eval::EvalFunc, true), consolePlayerCreater());

	Game g(minimaxPlayerCreaterCreater(9, Eval::EvalFunc, true),
	       minimaxPlayerCreaterCreater(9, Eval::EvalFunc, true));

	std::vector<std::string> plyStrs = {
		"F2F3",	  "C5C4", // 1
		"E2E3",	  "C6C5", // 1
		"F3F4",	  "D5D4", // 1
		"F4E5",	  "D6E5", // 1
		"B2B3",	  "D4E3", // 1
		"C1B2",	  "C4C3", // 1
		"B1C3",	  "E5D6", // 1
		"E1D3",	  "E3D2", // 1
		"D3C5",	  "D6C5", // 1
		"C3E4",	  "F5E4", // 1
		"F1F6",	  "B6D5", // 1
		"B2E5",	  "E4E3", // 1
		"F6F5",	  "A6D6", // 1
		"E5E3",	  "C5C6", // 1
		"E3E5",	  "E6F4", // 1
		"F5F4",	  "D5F4", // 1
		"E5F4",	  "D6D5", // 1
		"F4F6",	  "C6C5", // 1
		"C2C3",	  "D5D6", // 1
		"F6F5",	  "D6D5", // 1
		"F5F2",	  "C5C6", // 1
		"C3C4",	  "B5C4", // 1
		"B3C4",	  "D5D3", // 1
		"F2F5",	  "D3D4", // 1
		"F5B5",	  "C6D6", // 1
		"C4C5",	  "D6E5", // 1
		"C5C6=Q", "E5F4", // 1
				  // "B5A5",	  "D4D6", // 1
				  // "C6D6",	  "F4E4", // 1
				  // "D6D2",	  "E4F3", // 1
	};

	g.playPlyList(plyStrs, true, false);

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
