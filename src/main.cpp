#include "bitboard.h"
#include "constants.h"
#include "evaluation.h"
#include "magics.h"
#include "playing_interface.h"
#include "position.h"
#include "utils.h"

#include <algorithm>
#include <iostream>

auto minimaxPlayerCreaterCreater =
  [](int depth, std::function<Score(Position&)> evaluator, bool _verbose) {
	  return [depth, evaluator, _verbose]() {
		  return new ClassicalPlayer(depth, evaluator, _verbose);
	  };
  };

auto consolePlayerCreater = []() { return []() { return new ConsolePlayer(); }; };

int
main()
{
	auto consolePlayerCreater = []() { return []() { return new ConsolePlayer(); }; };
	Game g(minimaxPlayerCreaterCreater(9, Eval::EvalFunc, true), consolePlayerCreater());
	g.play();

	// std::cout << formatNumber(1233487) << std::endl;
	// std::cout << formatNumber(1227.2340234) << std::endl;
	// std::cout << formatNumber(27.123) << std::endl;
	// std::cout << formatNumber(88.9082, 3, 10) << std::endl;
	// std::cout << formatNumber(1088, 0, 10) << std::endl;

	return 0;
}

void
testFormatNumber()
{
	std::cout << formatNumber(1233487) << std::endl;
	std::cout << formatNumber(1227.2340234) << std::endl;
	std::cout << formatNumber(27.123) << std::endl;
	std::cout << formatNumber(88.9082, 3, 10) << std::endl;
	std::cout << formatNumber(1088, 0, 10) << std::endl;
}

void
demoPlayMinimax()
{
	Game g(minimaxPlayerCreaterCreater(8, Eval::EvalFunc, true),
	       minimaxPlayerCreaterCreater(8, Eval::EvalFunc, true));
	// std::vector<std::string> plyStrs = {
	// };
	// g.playPlyList(plyStrs, true, false);
	g.play();
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
