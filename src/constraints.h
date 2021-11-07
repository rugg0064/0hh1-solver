#ifndef _constraints_h
#define _constraints_h

#include "board.h"
uint8 passN2(Board *boardPtr, uint8 isCol, uint8 lineNum);
uint8 passRunOf3(Board *boardPtr, uint8 isCol, uint8 lineNum, uint8 start, uint8 stop);

// A board is identical only if it is full
uint8 isLineIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum1, uint8 lineNum2);
uint8 passIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum);

uint8 isLineValid(Board *boardPtr, uint8 isCol, uint8 lineNum);
//Checks an entire board for validity
uint8 isBoardValid(Board *boardPtr, uint8 flags);

//Would putting a value here break a rule
//uint8 canInsert(Board *boardPtr, uint8 x, uint8 y, uint8 type);

#endif