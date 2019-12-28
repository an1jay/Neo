#include "types.h"
#include <functional>
#include <iostream>

#pragma once

char *boardPrint(std::function<char(Square s)> mapper);

std::ostream &operator<<(std::ostream &os, Square s);

Square sqFromFileRank(const File f, const Rank r);

File fileFromSq(const Square s);

Rank rankFromSq(const Square s);