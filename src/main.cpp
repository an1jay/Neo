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
	auto consolePlayerCreater = []() { return []() { return new ConsolePlayer(); }; };

	auto minimaxPlayerCreaterCreater =
	  [](int depth, std::function<Score(Position&)> evaluator, bool _verbose) {
		  return [depth, evaluator, _verbose]() {
			  return new ClassicalPlayer(depth, evaluator, _verbose);
		  };
	  };

	Game g(minimaxPlayerCreaterCreater(6, Eval::EvalFunc, true), consolePlayerCreater());

	// Game g(minimaxPlayerCreaterCreater(5, Eval::EvalFunc, true),
	//        minimaxPlayerCreaterCreater(5, Eval::EvalFunc, true));

	// std::vector<std::string> plyStrs = {
	// 	"F2F3",	  "C5C4", // 1
	// 	"E2E3",	  "C6C5", // 1
	// 	"F3F4",	  "D5D4", // 1
	// 	"F4E5",	  "D6E5", // 1
	// 	"E3D4",	  "E6D4", // 1
	// 	"D2D3",	  "B6A4", // 1
	// 	"C2C3",	  "E5D5", // 1
	// 	"C3D4",	  "C5D4", // 1
	// 	"B1C3",	  "A4C3", // 1
	// 	"B2C3",	  "D4E5", // 1
	// 	"D3D4",	  "E5E6", // 1
	// 	"F1F3",	  "F5F4", // 1
	// 	"F3F4",	  "B5B4", // 1
	// 	"C3B4",	  "F6F4", // 1
	// 	"C1F4",	  "E6F6", // 1
	// 	"F4F6",	  "A6F6", // 1
	// 	"B4A5",	  "F6A6", // 1
	// 	"A2A3",	  "D5D4", // 1
	// 	"E1C2",	  "D4C3", // 1
	// 	"A3A4",	  "A6A5", // 1
	// 	"D1C1",	  "C3D3", // 1
	// 	"C2B4",	  "D3E3", // 1
	// 	"C1C2",	  "E3E4", // 1
	// 	"C2C3",	  "E4F3", // 1
	// 	"B4C6",	  "A5A6", // 1
	// 	"A4A5",	  "A6C6", // 1
	// 	"A5A6=Q", "C6A6", // 1
	// 	"A1A6",	  "F3E4", // 1
	// 	"A6F6",	  "E4E5", // 1
	// 	"F6C6",	  "E5E4", // 1
	// 	"C6F6",	  "E4E5", // 1
	// 	"F6C6",	  "E5E4", // 1
	// 	"C6E6",	  "E4D5", // 1
	// 	"E6B6",	  "D5E4", // 1
	// 	"B6E6",	  "E4D5", // 1
	// 	"E6B6",	  "D5E4", // 1
	// 	"B6D6",	  "E4E5", // 1
	// 	"D6A6",	  "E5E4", // 1
	// 	"A6D6",	  "E4E5", // 1
	// 	"D6A6",	  "E5E4", // 1
	// };

	// g.playPlyList(plyStrs, true, false);

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
