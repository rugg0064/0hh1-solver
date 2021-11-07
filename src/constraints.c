#include "constraints.h"

uint8 passN2(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	uint8 n = boardPtr->size;
	uint8 limit = n/2;
	uint8 redCount = 0;
	uint8 blueCount = 0;
	for(int i = 0; i < n; i++)
	{	
		uint8 isEmpty = isCol ? isSlotEmpty(boardPtr, lineNum, i) : isSlotEmpty(boardPtr, i, lineNum);
		if(!isEmpty)
		{
			uint8 value = isCol ? getColor(boardPtr, lineNum, i) : getColor(boardPtr, i, lineNum);
			redCount  += value ^ 1; //Value will return either 0/1 for red/blue
			//If the result was 0, then it was red, so if we flip the bit and add, we will add if it was red.
			blueCount += value;
		}
	}
	return (redCount <= limit) && (blueCount <= limit);
}

uint8 passRunOf3(Board *boardPtr, uint8 isCol, uint8 lineNum, uint8 start, uint8 stop)
{ //stop index is incusive!!!
  //A result of 1 means we did pass the run of 3 test.
	uint8 currentRunLength = 0;
	uint8 currentRunValue = 2;
	
	for(int i = start; i <= stop; i++)
	{
		uint8 isEmpty = isCol ? isSlotEmpty(boardPtr, lineNum, i) : isSlotEmpty(boardPtr, i, lineNum);
		
		if(isEmpty)
		{
			currentRunLength = 0;
		}
		else
		{
			uint8 value = isCol ? getColor(boardPtr, lineNum, i) : getColor(boardPtr, i, lineNum);
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

uint8 isLineIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum1, uint8 lineNum2)
{
	if(!isLineFull(boardPtr, isCol, lineNum1) != !isLineFull(boardPtr, isCol, lineNum2))
	{ //if line1 or line2 is isnt completely full, then we can't say its identical.
		return 0;
	}
	
	uint64 line1;
	uint64 line2;
	if(isCol)
	{
		line1 = boardPtr->colFirstBoard[lineNum1];
		line2 = boardPtr->colFirstBoard[lineNum2];
	}
	else
	{
		line1 = boardPtr->rowFirstBoard[lineNum1];
		line2 = boardPtr->rowFirstBoard[lineNum2];
	}
	return line1==line2;
}

uint8 passIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	uint8 n = boardPtr->size;
	for(int i = 0; i < n; i++)
	{
		if(i == n)
		{
			continue;
		}
		else
		{
			
		}
	}
	
}

uint8 isLineValid(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	return 0;
}

//Rule flags (there are 7)
//do full check
//identical rows
//identical cols
//n/2 of each row
//runs of 3 row
//n/2 of each col
//runs of 3 col
uint8 isBoardValid(Board *boardPtr, uint8 flags)
{
	/*
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
	*/
	return 1;
}