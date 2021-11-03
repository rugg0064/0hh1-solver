#ifndef _solver_h
#define _solver_h

#include "board.h"

Board solve(Board board);
int fixSandwiches(Board *boardPtr);
int fixRunsOfTwo(Board *boardPtr);
int fix2n(Board *boardPtr);

#endif