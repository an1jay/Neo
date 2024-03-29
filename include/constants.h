#include "types.h"

#pragma once

// ***** Primitives *****

constexpr int numSquaresInBoard = static_cast<int>(Square::NB_NONE);
constexpr int numSquaresInRankFile = static_cast<int>(File::NB_NONE);
constexpr int numSquaresShiftFactor = 64 - numSquaresInBoard;
constexpr int numEdges = 4;

constexpr Color Colors[] = { Color::Black, Color::White };
constexpr Square Squares[] = {
	Square::A1, Square::B1, Square::C1, Square::D1, Square::E1, Square::F1,
	Square::A2, Square::B2, Square::C2, Square::D2, Square::E2, Square::F2,
	Square::A3, Square::B3, Square::C3, Square::D3, Square::E3, Square::F3,
	Square::A4, Square::B4, Square::C4, Square::D4, Square::E4, Square::F4,
	Square::A5, Square::B5, Square::C5, Square::D5, Square::E5, Square::F5,
	Square::A6, Square::B6, Square::C6, Square::D6, Square::E6, Square::F6,
};
constexpr File Files[] = {
	File::A, File::B, File::C, File::D, File::E, File::F,
};
constexpr Rank Ranks[] = {
	Rank::First, Rank::Second, Rank::Third, Rank::Fourth, Rank::Fifth, Rank::Sixth,
};

// ***** Display *****

constexpr char FileChars[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
constexpr char RankChars[] = { '1', '2', '3', '4', '5', '6' };

constexpr char PieceChars[] = {
	'P', 'N', 'R', 'Q', 'K', 'p', 'n', 'r', 'q', 'k', ' ',
};

constexpr char PieceTypeChars[] = {
	'P', 'N', 'R', 'Q', 'K', ' ',
};

const Ply INVALID_PLY = 0ULL;

// ***** BitBoard *****

// Equal to 0000000000000000000000000000111111111111111111111111111111111111
constexpr BitBoard AllSquares = 68719476735;
constexpr BitBoard NoSquares = 0;

// BitBoards for each file
constexpr BitBoard FileA = 1090785345;
constexpr BitBoard FileB = 2181570690;
constexpr BitBoard FileC = 4363141380;
constexpr BitBoard FileD = 8726282760;
constexpr BitBoard FileE = 17452565520;
constexpr BitBoard FileF = 34905131040;

// BitBoards for each rank
constexpr BitBoard RankFirst = 63;
constexpr BitBoard RankSecond = 4032;
constexpr BitBoard RankThird = 258048;
constexpr BitBoard RankFourth = 16515072;
constexpr BitBoard RankFifth = 1056964608;
constexpr BitBoard RankSixth = 67645734912;

constexpr BitBoard RankBitBoards[] = {
	RankFirst, RankSecond, RankThird, RankFourth, RankFifth, RankSixth,
};

constexpr BitBoard FileBitBoards[] = {
	FileA, FileB, FileC, FileD, FileE, FileF,
};

constexpr BitBoard Edges = FileA | FileF | RankFirst | RankSixth;
constexpr BitBoard NoEdges = ~Edges;
constexpr BitBoard EdgeBitBoards[] = { FileA, FileF, RankFirst, RankSixth };

namespace AttackVectors {

constexpr BitBoard Rook[] = {
	1090785406,  2181570749,  4363141435,  8726282807,  17452565551, 34905131039,
	1090789249,  2181574466,  4363144900,  8726285768,  17452567504, 34905130976,
	1091035201,  2181812354,  4363366660,  8726475272,  17452692496, 34905126944,
	1106776129,  2197037186,  4377559300,  8738603528,  17460691984, 34904868896,
	2114195521,  3171426434,  5285888260,  9514811912,  17972659216, 34888353824,
	66589036609, 65532338306, 63418941700, 59192148488, 50738562064, 33831389216,
}; // tested to be correct

constexpr BitBoard Bishop[] = {
	34630287488, 541098304,	  8458880,     402688,	    17320448,	 1108378624,
	17315143682, 34630291461, 541368330,   25772052,    1108508712,	 2216755216,
	8657567876,  17315397960, 34647573137, 1649411362,  2225080836,	 4433380360,
	4328530184,  8673841680,  18421425248, 36842850433, 4966220034,	 8858436100,
	2148024848,  5370053664,  10740111360, 21480218688, 42960175233, 17184096514,
	34636832,    86050816,	  172359680,   344461312,   672145472,	 270549121,
}; // tested to be correct

constexpr BitBoard BishopMagicsOccupancyMask[] = {
	270549120, 4227328,   66176,	 9472,	    541184,    34636800,  135274496, 270548992,
	4235264,   606208,    34635776,	 69271552,  67633280,  135266560, 271057536, 38798592,
	69206528,  138413056, 33562880,	 67125760,  167814144, 335626368, 134250752, 268501504,
	541184,	   1082368,   2686976,	 5251072,   2113664,   4227328,	  34636800,  69271552,
	171966464, 336068608, 135274496, 270549120,

}; // generated from utils.cpp / genBishopMagicOccupancyMask(Square s)
// tested to be correct

constexpr BitBoard RookMagicsOccupancyMask[] = {
	17043550,    34087068,	  68174106,    136348182,   272696334,	 545392670,
	17045376,    34088704,	  68175488,    136349056,   272696192,	 545392512,
	17162304,    34193536,	  68264192,    136405504,   272688128,	 545384448,
	24645696,    40902784,	  73941248,    140018176,   272172032,	 544868352,
	503582784,   470294656,	  437272832,   371229184,   239141888,	 511838208,
	32229298240, 30098858112, 27985461504, 23758668288, 15305081856, 32757647360,
}; // generated from utils.cpp / genRookMagicOccupancyMask(Square s)
// tested to be correct

constexpr BitBoard King[] = {
	194,	    453,	906,	     1812,	  3624,	       3088,
	12419,	    28999,	57998,	     115996,	  231992,      197680,
	794816,	    1855936,	3711872,     7423744,	  14847488,    12651520,
	50868224,   118779904,	237559808,   475119616,	  950239232,   809697280,
	3255566336, 7601913856, 15203827712, 30407655424, 60815310848, 51820625920,
	2197815296, 5486149632, 10972299264, 21944598528, 43889197056, 17985175552,
};

constexpr BitBoard Knight[] = {
	8448,	    20992,	42048,	     84096,	  164096,      66048,
	540676,	    1343496,	2691089,     5382178,	  10502148,    4227080,
	34603266,   85983749,	172229706,   344459412,	  672137512,   270533136,
	2214609024, 5502959936, 11022701184, 22045402368, 43016800768, 17314120704,
	4296024064, 8592052224, 18258108416, 36516216832, 4296179712,  8592097280,
	67633152,   135528448,	287834112,   575668224,	  77594624,    138412032,
};

constexpr BitBoard Queen[] = {
	35721072894, 2722669053,  4371600315,  8726685495,  17469885999, 36013509663,
	18405932931, 36811865927, 4904513230,  8752057820,  18561076216, 37121886192,
	9748603077,  19497210314, 39010939797, 10375886634, 19677773332, 39338507304,
	5435306313,  10870878866, 22798984548, 45581453961, 22426912018, 43763304996,
	4262220369,  8541480098,  16025999620, 30995030600, 60932834449, 52072450338,
	66623673441, 65618389122, 63591301380, 59536609800, 51410707536, 34101938337,
}; // tested to be correct

constexpr BitBoard WhitePawnPush[] = {
	64,	    128,	256,	    512,	1024,	     2048,
	4096,	    8192,	16384,	    32768,	65536,	     131072,
	262144,	    524288,	1048576,    2097152,	4194304,     8388608,
	16777216,   33554432,	67108864,   134217728,	268435456,   536870912,
	1073741824, 2147483648, 4294967296, 8589934592, 17179869184, 34359738368,
	0,	    0,		0,	    0,		0,	     0,
};

constexpr BitBoard WhitePawnCapture[] = {
	128,	    320,	640,	     1280,	  2560,	       1024,
	8192,	    20480,	40960,	     81920,	  163840,      65536,
	524288,	    1310720,	2621440,     5242880,	  10485760,    4194304,
	33554432,   83886080,	167772160,   335544320,	  671088640,   268435456,
	2147483648, 5368709120, 10737418240, 21474836480, 42949672960, 17179869184,
	0,	    0,		0,	     0,		  0,	       0,
};

constexpr BitBoard BlackPawnPush[] = {
	0,	 0,	  0,	   0,	     0,	       0,	 1,	    2,	       4,
	8,	 16,	  32,	   64,	     128,      256,	 512,	    1024,      2048,
	4096,	 8192,	  16384,   32768,    65536,    131072,	 262144,    524288,    1048576,
	2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912,
};

constexpr BitBoard BlackPawnCapture[] = {
	0,	 0,	   0,	    0,	      0,	0,	   2,	      5,	 10,
	20,	 40,	   16,	    128,      320,	640,	   1280,      2560,	 1024,
	8192,	 20480,	   40960,   81920,    163840,	65536,	   524288,    1310720,	 2621440,
	5242880, 10485760, 4194304, 33554432, 83886080, 167772160, 335544320, 671088640, 268435456,
};

constexpr BitBoard Diagonals[] = {
	34630287488, 541098240,	  8454656,     132096,	    2048,	 0,
	17315143680, 34630287361, 541097986,   8454148,	    131080,	 16,
	8657567744,  17315135552, 34630271105, 541065474,   8389124,	 1032,
	4328521728,  8657047552,  17314095168, 34628190337, 536903938,	 66052,
	2147483648,  4295229440,  8590462976,  17180926016, 34361852033, 4227330,
	0,	     16777216,	  33816576,    67637248,    135274560,	 270549121,
};

constexpr BitBoard AntiDiagonals[] = {
	0,	  64,	      4224,	  270592,     17318400,	  1108378624,
	2,	  4100,	      270344,	  17317904,   1108377632, 2216755200,
	132,	  262408,     17302032,	  1108345888, 2216691712, 4433379328,
	8456,	  16794128,   1107330080, 2214660096, 4429316096, 8858370048,
	541200,	  1074824224, 2149648384, 4299292672, 8598323200, 17179869184,
	34636832, 69273600,   138543104,  276824064,  536870912,  0,
};
} // namespace AttackVectors