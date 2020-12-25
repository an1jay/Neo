# Neo is a Los Alamos Chess Engine

## To do
- [ ] Framework
  - [ ] Board representation
    - [ ] Position
      - [x] calculate Checkers
      - [ ] do / undo move hashing
      - [ ] generate Pseudo Legal Moves
      - [ ] generate Legal Moves
      - [ ] calculate Game Result
      - [ ] is Valid
    - [ ] Move generation
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


## Sources

- catch.h - https://github.com/catchorg/Catch2
- colors.h - slightly modified version of https://github.com/gon1332/fort320/blob/master/include/Utils/colors.h