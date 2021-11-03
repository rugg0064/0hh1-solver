#ifndef _board_h
#define _board_h

#include "uints.h"
typedef struct board 
{
	uint8 size;
	uint64 rowFirstBoard[64];
	uint64 colFirstBoard[64];
} Board;

int isBoardFull(Board *boardPtr);
Board createBoard(uint8 size);
void setValue(Board *boardPtr, uint8 x, uint8 y, uint8 value);
uint8 getValue(Board *boardPtr, uint8 x, uint8 y);

uint8 isEmpty(Board *boardPtr, uint8 x, uint8 y);
uint8 isLineFull(Board *boardPtr, uint8 isCol, uint8 lineValue);
uint8 isBoardFull(Board *boardPtr);

uint8 anyIdenticalRows(Board *boardPtr, uint8 x);
uint8 anyIdenticalCols(Board *boardPtr, uint8 y);

/* Data helpers */
uint8 getNumOfColorInRow(Board *boardPtr, uint8 y, uint8 type);
uint8 getNumOfColorInCol(Board *boardPtr, uint8 x, uint8 type);

void printBoard(Board *boardPtr);

#endif