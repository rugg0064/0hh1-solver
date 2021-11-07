#ifndef _board_h
#define _board_h

#include "uints.h"
typedef struct board 
{
	uint8 size;
	uint64 rowFirstBoard[64];
	uint64 colFirstBoard[64];
} Board;

Board createBoard(uint8 size);

void setFilled(Board *boardPtr, uint8 x, uint8 y, uint8 isFilled);
void setColor(Board *boardPtr, uint8 x, uint8 y, uint8 color);
uint8 getColor(Board *boardPtr, uint8 x, uint8 y);

uint8 isSlotEmpty(Board *boardPtr, uint8 x, uint8 y);
uint8 isLineFull(Board *boardPtr, uint8 isCol, uint8 lineNum);
uint8 isBoardFull(Board *boardPtr);

/* Data helpers */
uint8 getNumOfColorInLine(Board *boardPtr, uint8 isCol, uint8 lineNum, uint8 type);

void printBoard(Board *boardPtr);

#endif