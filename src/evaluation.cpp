#include "evaluation.h"

#include "position.h"

namespace Eval {

Score
EvalFunc(Position& pos)
{
	assert(pos.currentGameResult() == GameResult::NB_NONE);
	Score eval = 0;
	// for now material only
	for (int i = 0; i < static_cast<int>(Piece::NB_NONE); i++) {
		eval += Eval::materialWeights[i] * pos.pieceCount(static_cast<Piece>(i));
	}
	return eval;
}

} // namespace Eval