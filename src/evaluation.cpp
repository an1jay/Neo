#include "evaluation.h"
#include "position.h"


namespace Eval {

Score
EvalFunc(Position& pos)
{
	GameResult gr = pos.currentGameResult();
	Score eval = 0;
	if (gr == GameResult::NB_NONE) {
		// for now material only
		for (int i = 0; i < static_cast<int>(Piece::NB_NONE); i++) {
			eval += Eval::materialWeights[i] * pos.pieceCount(static_cast<Piece>(i));
		}
	} else {
		if (gr == GameResult::BlackWin)
			return Eval::BlackWinScore;
		if (gr == GameResult::WhiteWin)
			return Eval::WhiteWinScore;
		if (gr == GameResult::Draw)
			return 0;
	}
	return eval;
}

} // namespace Eval