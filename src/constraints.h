#ifndef _constraints_h
#define _constraints_h

#include "board.h"
int passN2Rule(Board *boardPtr, int doCol, int line, int start, int stop);
int passRunOf3(Board *boardPtr, int doCol, int line, int start, int stop);
int hasIdenticalLine(Board *boardPtr, int doCol, int line);

uint8 passN2(Board *boardPtr, uint8 isCol, uint8 lineNum);
uint8 passRunOf3(Board *boardPtr, uint8 isCol, uint8 lineNum, uint8 start, uint8 stop);

uint8 isIdentica(Board *boardPtr, uint8 isCol, uint8 lineNum1, uint8 lineNum2);
uint8 passIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum);


uint8 passN2Row(Board *boardPtr, int y);
uint8 passN2Col(Board *boardPtr, int x);
uint8 passNoIdenticalRows(Board *boardPtr);
uint8 passNoIdenticalCols(Board *boardPtr);
uint8 passNoRunsOf3Row(Board *boardPtr, int y);
uint8 passNoRunsOf3Col(Board *boardPtr, int x);

//Checks an entire board for validity
int isBoardValid(Board *boardPtr, uint8 flags);

//Would putting a value here break a rule
uint8 canInsert(Board *boardPtr, uint8 x, uint8 y, uint8 type);

#endif