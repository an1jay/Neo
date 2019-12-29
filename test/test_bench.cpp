#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.h"

#include "magics.h"
#include <random>

TEST_CASE("Attack Vector Generation")
{

	Magics mag = Magics();
	std::mt19937 rng = std::mt19937();
	BitBoard Occupancy = genRand(rng) & AllSquares;
	// BENCHMARK("HQ movegen") { return mag.HQBishopAttack(Square::F1, Occupancy); };
	// BENCHMARK("Magic movegen") { return mag.MagicBishopAttack(Square::F1, Occupancy); };
	// TODO: Figure out why this doesn't work
}
