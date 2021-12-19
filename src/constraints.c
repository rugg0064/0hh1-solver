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

uint8 passN2All(Board *boardPtr, uint8 isCol)
{
	uint8 n = boardPtr->size;
	for(int i = 0; i < n; i++)
	{
		if(!passN2(boardPtr, isCol, i))
		{
			return 0;
		}
	}
	return 1;
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

uint8 passRunOf3All(Board *boardPtr, uint8 isCol)
{
	uint8 n = boardPtr->size;
	for(int i = 0; i < n; i++)
	{
		if(!passRunOf3(boardPtr, isCol, i, 0, n-1))
		{
			return 0;
		}
	}
	return 1;
}

uint8 isLineIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum1, uint8 lineNum2)
{
	if(!isLineFull(boardPtr, isCol, lineNum1) || !isLineFull(boardPtr, isCol, lineNum2))
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

uint8 lineHasIdentical(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	uint8 n = boardPtr->size;
	for(int i = 0; i < n; i++)
	{
		if(i == lineNum)
		{
			continue;
		}
		else
		{
			if(isLineIdentical(boardPtr, isCol, lineNum, i))
			{
				return 0;
			}
		}
	}
	return 1;
}

uint8 passIdentical(Board *boardPtr, uint8 isCol)
{
	uint8 n = boardPtr->size;
	for(int i = 0; i < n-1; i++)
	{
		for(int j = i+1; j < n; j++)
		{
			if(isLineIdentical(boardPtr, isCol, i, j))
			{
				return 0;
			}
		}
	}
	return 1;
}

uint8 isLineValid(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	uint8 n = boardPtr->size;
	uint8 passRun = passRunOf3(boardPtr, isCol, lineNum, 0, n-1);
	uint8 passIdentical = !lineHasIdentical(boardPtr, isCol, lineNum);
	uint8 didPassN2 = passN2(boardPtr, isCol, lineNum);
	if(!passRun || !passIdentical || !didPassN2)
	{
		return 0;
	}
	return 1;
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
	int n = boardPtr->size;
	
	//1 means it passed the result
	uint8 results = 0;
	results |= (passRunOf3All(boardPtr, 1) << 0);
	results |= (passN2All(boardPtr, 1) << 1);
	results |= (passRunOf3All(boardPtr, 0) << 2);
	results |= (passN2All(boardPtr, 0) << 3);
	results |= (passIdentical(boardPtr, 0) << 4);
	results |= (passIdentical(boardPtr, 1) << 5);
	results |= (isBoardFull(boardPtr) << 6);
	
	//Since a flag bit being set to 1 means to care about the test
	//And a flag bit of 0 means ignore.
	//We can flip the flag bits and OR them to force the
	//Bits which we dont care about to 1, as if they passed
	return (results | (~flags & 0b1111111)) == 0b1111111;
}