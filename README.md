# Neo is a Los Alamos Chess Engine

## To do
- [ ] Framework
  - [ ] Board representation
    - [ ] Position
      - [x] calculate Checkers
      - [x] do / undo move hashing
      - [x] generate Pseudo Legal Moves
      - [x] generate Legal Moves
      - [x] calculate Game Result
      - [ ] is Valid
    - [x] Move generation
      - [x] Incorporate magic bitboards
      - [x] Pawns
      - [x] Knights / King
    - [ ] Game playing
- [ ] Testing
  - [ ] Pawn Pushes & Capture AttackVectors
  - [ ] Knight & King Moves AttackVectors
  - [ ] Diagonals & Anti Diagonals AttackVectors
- [ ] AI
  - [ ] TBD.

## Possible Extensions
- [ ] Faster Zobrist 3 fold repetition detection - https://marcelk.net/2013-04-06/paper/upcoming-rep-v2.pdf
- [ ] Pre-allocate StateInfos? - don't allocate / free in the hotpath
- [ ] remove `GameResult _gameResult = calculateGameResult();` from `Position::doPly` - move search will already do this

## Sources

- catch.h - https://github.com/catchorg/Catch2
- colors.h - slightly modified version of https://github.com/gon1332/fort320/blob/master/include/Utils/colors.h