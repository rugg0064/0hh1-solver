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
	}
	return board;
}

void setValue(Board *boardPtr, uint8 x, uint8 y, uint8 value)
{ //Value should be 2 bits, 0b11 for instance, significant bit being EXISTS bit, LSB being the value
	uint64 row = boardPtr->rowFirstBoard[y];
	row = uint64WithBit(row, 1, (x*2)    , value >> 1); //Sets exists bit to MSB
	row = uint64WithBit(row, 1, (x*2) + 1, value & 1); //Sets value bit to LSB
	boardPtr->rowFirstBoard[y] = row; //Update the actual row
	
	uint64 col = boardPtr->colFirstBoard[x];
	col = uint64WithBit(col, 1, (y*2)    , value >> 1); //Sets exists bit to MSB
	col = uint64WithBit(col, 1, (y*2) + 1, value & 1); //Sets value bit to LSB
	boardPtr->colFirstBoard[x] = col; //Update the actual col
}

uint8 isEmpty(Board *boardPtr, uint8 x, uint8 y)
{
	uint64 row = boardPtr->rowFirstBoard[y];
	uint8 bitOffset = 63 - (x*2);
	return getBit(row, 1, bitOffset);
}

int isBoardFull(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	int full = 1;
	for(int x = 0; x < n; x++)
	{
		for(int y = 0; y < n; y++)
		{	
			full &= !isEmpty(boardPtr, x, y);
		}
	}
	return full;
}

uint8 getValue(Board *boardPtr, uint8 x, uint8 y)
{
	Board board = *boardPtr;
	uint64 row = board.rowFirstBoard[y];
	int bitOffset = 62 - (x*2);
	//The position of the bit we want is <bitCount> - 2 - (2x) left from the LSB
	return (row & (((uint64)1) << bitOffset) ) >> bitOffset;
}

uint8 getNumOfColorInRow(Board *boardPtr, uint8 y, uint8 type)
{
	uint8 n = boardPtr->size;
	int count = 0;
	for(int x = 0; x < n; x++)
	{
		if(!isEmpty(boardPtr, x, y))
		{
			count += getValue(boardPtr, x, y) == type;
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
		if(!isEmpty(boardPtr, x, y))
		{
			count += getValue(boardPtr, x, y) == type;
		}
	}
	return count;
}

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
		if(!isEmpty(boardPtr, i, y))
		{
			uint8 value = getValue(boardPtr, i, y);
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
		if(!isEmpty(boardPtr, x, i))
		{
			uint8 value = getValue(boardPtr, x, i);
			//Remember, a result of 0b00 is red. That means if you flip the LSB and add
			//	That is 1 if and only if the result was red.
			redCount  += value ^ 1;
			//Conversely, if it is 0b01, then it is blue.
			blueCount += value;
		}
	}
	return (redCount <= limit) && (blueCount <= limit);
}

uint8 passNoIdenticalRows(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int y = 0; y < n; y++)
	{
		if(anyIdenticalRows(boardPtr, y))
		{
			return 0;
		}
	}
	return 1;
}

uint8 passNoIdenticalCols(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int x = 0; x < n; x++)
	{
		if(anyIdenticalCols(boardPtr, x))
		{
			return 0;
		}
	}
	return 1;
}

uint8 passNoRunsOf3Row(Board *boardPtr, int y)
{
	uint8 n = boardPtr->size;
	uint8 currentRunLength = 0;
	uint8 currentRunValue = 2;
	
	for(int x = 0; x < n; x++)
	{
		if(isEmpty(boardPtr, x, y))
		{
			currentRunLength = 0;
		}
		else
		{
			uint8 value = getValue(boardPtr, x, y);
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
		if(isEmpty(boardPtr, x, y))
		{
			currentRunLength = 0;
		}
		else
		{
			uint8 value = getValue(boardPtr, x, y);
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

void printBoard(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int y = 0; y < n; y++)
	{
		for(int x = 0; x < n; x++)
		{
			if(isEmpty(boardPtr, x, y))
			{
				printf("_");
			}
			else
			{
				if(getValue(boardPtr, x, y))
				{
					printf("B");
				}
				else
				{
					printf("R");
				}
			}
		}
		printf("\n");
	}
}

//Rule flags (there are 7)
//do full check
//identical rows
//identical cols
//n/2 of each row
//runs of 3 row
//n/2 of each col
//runs of 3 col
int isBoardValid(Board *boardPtr, uint8 flags)
{
	int n = boardPtr->size;
	
	if( (flags >> 6) & 1)
	{
		if(!isBoardFull(boardPtr))
		{
			//printf("Board is not full\n");
			return 0;
		}
	}
	
	if( (flags >> 5) & 1)
	{
		if(!passNoIdenticalRows(boardPtr))
		{
			//printf("Some identical row\n");
			return 0;
		}
	}
	
	if( (flags >> 4) & 1)
	{
		if(!passNoIdenticalCols(boardPtr))
		{
			//printf("Some identical col\n");
			return 0;
		}
	}
	
	for(int x = 0; x < n; x++)
	{
		if( (flags >> 3) & 1)
		{
			if(!passN2Row(boardPtr, x))
			{
				//printf("n/2 rule on row\n");
				return 0;
			}
		}
		if( (flags >> 2) & 1)
		{
			if(!passNoRunsOf3Row(boardPtr, x))
			{
				//printf("run of 3 on row\n");
				return 0;
			}
		}
	}
	
	for(int y = 0; y < n; y++)
	{
		if( (flags >> 1) & 1)
		{
			if(!passN2Col(boardPtr, y))
			{
				//printf("n/2 rule on col\n");
				return 0;
			};
		}
		
		if( (flags >> 0) & 1)
		{
			if(!passNoRunsOf3Col(boardPtr, y))
			{
				//printf("run of 3 on col\n");
				return 0;
			}
		}
	}
	return 1;
}


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
			int y1Empty = isEmpty(boardPtr, x, y);
			int y2Empty = isEmpty(boardPtr, x, y2);
			if(y1Empty || y2Empty)
			{
				allSame = 0;
			}
			if(getValue(boardPtr, x, y) != getValue(boardPtr, x, y2))
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
			int x1Empty = isEmpty(boardPtr, x, y);
			int x2Empty = isEmpty(boardPtr, x2, y);
			if(x1Empty || x2Empty)
			{
				allSame = 0;
			}
			if(getValue(boardPtr, x, y) != getValue(boardPtr, x2, y))
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
	int empty = isEmpty(boardPtr, x, y);
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
		if(!isEmpty(boardPtr, x1, y))
		{
			if(getValue(boardPtr, x1, y) == type)
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
		if(!isEmpty(boardPtr, x, y1))
		{
			if(getValue(boardPtr, x, y1) == type)
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


