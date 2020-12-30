# Neo is a Los Alamos Chess Engine

## To do
- [ ] Framework
  - [ ] Board representation
    - [ ] Position
      - [ ] calculate Checkers -- Some Positions get misidentified as draw when not a draw - example game:

        Move List: 
        1    F2F3  |  C5C4  
        2    F3F4  |  E5F4  
        3    B2B3  |  C6C5  
        4    C1A3  |  D5D4  
        5    B3C4  |  B6C4  
        6    A3C5  |  D6C5  
        7    C2C3  |  C5D5  
        8    C3D4  |  B5B4  
        9    A2A3  |  E6D4  
        10   A3B4  |  A5B4  
        11   A1A6  |  F6A6  
        12   E1D3  |  A6A1  
        13   D3B4  |  D5D6  
        14   B4C2  |  A1B1  


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
- [ ] Lightweight Position - without magics? Or make positions share magics?
- [ ] Classical Player - hardcode side to play?

## Sources

- catch.h - https://github.com/catchorg/Catch2
- colors.h - slightly modified version of https://github.com/gon1332/fort320/blob/master/include/Utils/colors.h