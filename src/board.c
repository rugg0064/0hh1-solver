#include "board.h"

#include <stdio.h>
#include <string.h>
#include "uints.h"

//0,0 is the top-left, X goes to the right, Y goes down.
//0 is red
//1 is blue
//FILE INPUT MUST BE CR+LF LINE ENDING

Board createBoard(uint8 size)
{
	Board board;
	board.size = size;
	for(int i = 0; i < size; i++)
	{
		board.rowFirstBoard[i] = 0;
		board.colFirstBoard[i] = 0;
	}
	return board;
}

void setFilled(Board *boardPtr, uint8 x, uint8 y, uint8 isFilled)
{
	boardPtr->rowFirstBoard[y] = uint64WithBit(boardPtr->rowFirstBoard[y], 1, x, isFilled);
	boardPtr->colFirstBoard[x] = uint64WithBit(boardPtr->colFirstBoard[x], 1, y, isFilled);
}

void setColor(Board *boardPtr, uint8 x, uint8 y, uint8 color)
{
	boardPtr->rowFirstBoard[y] = uint64WithBit(boardPtr->rowFirstBoard[y], 0, x, color);
	boardPtr->colFirstBoard[x] = uint64WithBit(boardPtr->colFirstBoard[x], 0, y, color);	
}

uint8 isSlotEmpty(Board *boardPtr, uint8 x, uint8 y)
{
	return uint64GetBit(boardPtr->rowFirstBoard[y], 1, x);
}

uint8 getColor(Board *boardPtr, uint8 x, uint8 y)
{
	uint64 row = boardPtr->rowFirstBoard[y];
	return uint64GetBit(row, 0, x);
}

uint8 isLineFull(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	//Since the uint64 for a line is {32xFilled}{32xColor}
	//We can do a simple check to see if the top N bits are all 1
	uint8 n = boardPtr->size;
	uint64 line;
	if(isCol)
	{
		line = boardPtr->colFirstBoard[lineNum];
	}
	else
	{
		line = boardPtr->colFirstBoard[lineNum];
	}
	uint64 mask = uint64max;
	line >>= 64-n;
	mask >>= 64-n;
	return line==mask;
}

uint8 isBoardFull(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int i = 0; i < n; i++)
	{
		if(!isLineFull(boardPtr, 0, i))
		{
			return 0;
		}
	}
	return 1;
}

uint8 getNumOfColorInRow(Board *boardPtr, uint8 y, uint8 type)
{
	uint8 n = boardPtr->size;
	int count = 0;
	for(int x = 0; x < n; x++)
	{
		if(!isSlotEmpty(boardPtr, x, y))
		{
			count += getColor(boardPtr, x, y) == type;
		}
	}
	return count;
}
uint8 getNumOfColorInCol(Board *boardPtr, uint8 x, uint8 type)
{
	uint8 n = boardPtr->size;
	int count = 0;
	for(int y = 0; y < n; y++)
	{
		if(!isSlotEmpty(boardPtr, x, y))
		{
			count += getColor(boardPtr, x, y) == type;
		}
	}
	return count;
}

/*
uint8 passN2Row(Board *boardPtr, int y)
{ //Checks for the rule "There must be the same number of red and blue tiles in any row"
	//More accurately, this should say something like "The most amount of any color is n/2"
	//uint64 row = boardPtr->rowFirstBoard[y];
	uint8 n = boardPtr->size;
	uint8 limit = n/2;
	uint8 redCount = 0;
	uint8 blueCount = 0;
	for(int i = 0; i < n; i++)
	{	
		if(!isSlotEmpty(boardPtr, i, y))
		{
			uint8 value = getColor(boardPtr, i, y);
			//Remember, a result of 0b00 is red. That means if you flip the LSB and add
			//	That is 1 if and only if the result was red.
			redCount  += value ^ 1;
			//Conversely, if it is 0b01, then it is blue.
			blueCount += value;
		}
	}
	return (redCount <= limit) && (blueCount <= limit);
}

uint8 passN2Col(Board *boardPtr, int x)
{ //Checks for the rule "There must be the same number of red and blue tiles in any col"
	//Basically the same as above.
	//uint64 col = boardPtr->colFirstBoard[x];
	uint8 n = boardPtr->size;
	uint8 limit = n/2;
	uint8 redCount = 0;
	uint8 blueCount = 0;
	for(int i = 0; i < n; i++)
	{	
		if(!isSlotEmpty(boardPtr, x, i))
		{
			uint8 value = getColor(boardPtr, x, i);
			//Remember, a result of 0b00 is red. That means if you flip the LSB and add
			//	That is 1 if and only if the result was red.
			redCount  += value ^ 1;
			//Conversely, if it is 0b01, then it is blue.
			blueCount += value;
		}
	}
	return (redCount <= limit) && (blueCount <= limit);
}

uint8 passNoRunsOf3Row(Board *boardPtr, int y)
{
	uint8 n = boardPtr->size;
	uint8 currentRunLength = 0;
	uint8 currentRunValue = 2;
	
	for(int x = 0; x < n; x++)
	{
		if(isSlotEmpty(boardPtr, x, y))
		{
			currentRunLength = 0;
		}
		else
		{
			uint8 value = getColor(boardPtr, x, y);
			if(value != currentRunValue)
			{
				currentRunLength = 1;
				currentRunValue = value;
			}
			else
			{
				currentRunLength++;
			}
		}
		if(currentRunLength >= 3)
		{
			return 0;
		}
	}
	return 1;
}

uint8 passNoRunsOf3Col(Board *boardPtr, int x)
{
	uint8 n = boardPtr->size;
	uint8 currentRunLength = 0;
	uint8 currentRunValue = 2;
	
	for(int y = 0; y < n; y++)
	{
		if(isSlotEmpty(boardPtr, x, y))
		{
			currentRunLength = 0;
		}
		else
		{
			uint8 value = getColor(boardPtr, x, y);
			if(value != currentRunValue)
			{
				currentRunLength = 1;
				currentRunValue = value;
			}
			else
			{
				currentRunLength++;
			}
		}
		if(currentRunLength >= 3)
		{
			return 0;
		}
	}
	return 1;
}*/

void printBoard(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int y = 0; y < n; y++)
	{
		for(int x = 0; x < n; x++)
		{
			if(isSlotEmpty(boardPtr, x, y))
			{
				printf("_");
			}
			else
			{
				if(getColor(boardPtr, x, y))
				{
					printf("B");
				}
				else
				{
					printf("R");
				}
			}
			
			//printf("%u", uint64GetBit(boardPtr->rowFirstBoard[y], 1, x));
			//printf("%u", isSlotEmpty(boardPtr, x, y));
		}
		printf("\n");
	}
}



/*
uint8 anyIdenticalRows(Board *boardPtr, uint8 y)
{
	uint8 n = boardPtr->size;
	for(int y2 = 0; y2 < n; y2++)
	{
		if(y2 == y)
		{
			continue;
		}
		int allSame = 1;
		for(int x = 0; x < n && allSame; x++)
		{
			int y1Empty = isSlotEmpty(boardPtr, x, y);
			int y2Empty = isSlotEmpty(boardPtr, x, y2);
			if(y1Empty || y2Empty)
			{
				allSame = 0;
			}
			if(getColor(boardPtr, x, y) != getColor(boardPtr, x, y2))
			{
				allSame = 0;
			}
		}
		if(allSame)
		{
			return 1;
		}
	}
	return 0;
}

uint8 anyIdenticalCols(Board *boardPtr, uint8 x)
{
	uint8 n = boardPtr->size;
	for(int x2 = 0; x2 < n; x2++)
	{
		if(x2 == x)
		{
			continue;
		}
		int allSame = 1;
		for(int y = 0; y < n && allSame; y++)
		{
			int x1Empty = isSlotEmpty(boardPtr, x, y);
			int x2Empty = isSlotEmpty(boardPtr, x2, y);
			if(x1Empty || x2Empty)
			{
				allSame = 0;
			}
			if(getColor(boardPtr, x, y) != getColor(boardPtr, x2, y))
			{
				allSame = 0;
			}
		}
		if(allSame)
		{
			return 1;
		}
	}
	return 0;
}

uint8 canInsert(Board *boardPtr, uint8 x, uint8 y, uint8 type)
{
	int empty = isSlotEmpty(boardPtr, x, y);
	uint8 n = boardPtr->size;
	uint8 typeCountCol = getNumOfColorInCol(boardPtr, x, type);
	int n2ConstraintCol = typeCountCol < (n/2);
	
	uint8 typeCountRow = getNumOfColorInRow(boardPtr, x, type);
	int n2ConstraintRow = typeCountRow < (n/2);
	
	if(! (empty && n2ConstraintCol && n2ConstraintRow))
	{
		return 0;
	}
	
	//Do run of 3 check horizontals
	int startValue = x-2;
	if(startValue < 0)
	{
		startValue = 0;
	}
	int endValue = x+2;
	if(endValue >= n)
	{
		endValue = n-1;
	}
	int runCount = 0;
	for(int x1 = startValue; x1 <= endValue; x1++)
	{
		if(x1 == x)
		{
			runCount++;
			if(runCount >= 3)
			{
				return 0;
			}
			continue;
		}
		if(!isSlotEmpty(boardPtr, x1, y))
		{
			if(getColor(boardPtr, x1, y) == type)
			{
				runCount++;
				printf("%d\n", runCount);
				if(runCount >= 3)
				{
					return 0;
				}
			}
			else
			{
				runCount = 0;
			}
		}
		else
		{
			runCount = 0;
		}
	}
	
	//Do run of 3 check vertically
	startValue = y-2;
	if(startValue < 0)
	{
		startValue = 0;
	}
	endValue = y+2;
	if(endValue >= n)
	{
		endValue = n-1;
	}
	runCount = 0;
	for(int y1 = startValue; y1 <= endValue; y1++)
	{
		if(y1 == y)
		{
			runCount++;
			if(runCount >= 3)
			{
				return 0;
			}
			continue;
		}
		if(!isSlotEmpty(boardPtr, x, y1))
		{
			if(getColor(boardPtr, x, y1) == type)
			{
				runCount++;
				if(runCount >= 3)
				{
					return 0;
				}
			}
			else
			{
				runCount = 0;
			}
		}
		else
		{
			runCount = 0;
		}
	}
	return 1;
}
*/
