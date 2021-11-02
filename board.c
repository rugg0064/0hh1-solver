#include <stdio.h>
#include <stdint.h>

// 1111111111111111111111111111111111111111111111111111111111111111
// 0000000000000000000000000000000000000000000000000000000000000000

typedef uint_fast8_t uint8;
typedef uint_fast64_t uint64;

typedef struct board 
{
	uint8 size;
	uint64 rowFirstBoard[64];
	uint64 colFirstBoard[64];
} Board;
//0,0 is the top-left, X goes to the right, Y goes down.
//0 is red
//1 is blue
//For all constraints, 1 means it passed, 0 means it failed.
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

uint64 intWithBit(uint64 value, uint8 bitNumber, uint8 setToValue)
{
	uint8 bitOffset = 63 - bitNumber;
	uint64 bitMask = ((uint64)1) << bitOffset;
	
	if(setToValue)
	{ //If setting to 1
		return value | bitMask;
	}
	else
	{
		return value 
			& (0b1111111111111111111111111111111111111111111111111111111111111111
			   ^ bitMask 
			);
	}
}

void setPosition(Board *boardPtr, uint8 x, uint8 y, uint8 value)
{ //Value should be 2 bits, 0b11 for instance, significant bit being EXISTS bit, LSB being the value
	uint64 row = boardPtr->rowFirstBoard[y];
	row = intWithBit(row, (x*2)    , value >> 1); //Sets exists bit to MSB
	row = intWithBit(row, (x*2) + 1, value & 1); //Sets value bit to LSB
	boardPtr->rowFirstBoard[y] = row; //Update the actual row
	
	uint64 col = boardPtr->colFirstBoard[x];
	col = intWithBit(col, (y*2)    , value >> 1); //Sets exists bit to MSB
	col = intWithBit(col, (y*2) + 1, value & 1); //Sets value bit to LSB
	boardPtr->colFirstBoard[x] = col; //Update the actual col
}

int isEmpty(Board *boardPtr, uint8 x, uint8 y)
{
	uint64 row = boardPtr->rowFirstBoard[y];
	uint8 bitOffset = 63 - (x*2);
	//The position of the bit we want is <bitCount> - 1 - (2x) left from the LSB
	uint64 mask = ((uint64)1) << bitOffset;
	uint64 result = (row & mask) >> bitOffset;
	return !result;
}

uint8 getValue(Board *boardPtr, uint8 x, uint8 y)
{
	Board board = *boardPtr;
	uint64 row = board.rowFirstBoard[y];
	int bitOffset = 62 - (x*2);
	//The position of the bit we want is <bitCount> - 2 - (2x) left from the LSB
	return (row & (((uint64)1) << bitOffset) ) >> bitOffset;
}

uint8 passRowCountConstraint(Board *boardPtr, int y)
{ //Checks for the rule "There must be the same number of red and blue tiles in any row"
	//More accurately, this should say something like "The most amount of any color is n/2"
	uint64 row = boardPtr->rowFirstBoard[y];
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

uint8 passColCountConstraint(Board *boardPtr, int x)
{ //Checks for the rule "There must be the same number of red and blue tiles in any col"
	//Basically the same as above.
	uint64 col = boardPtr->colFirstBoard[x];
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

uint8 passNoSameRows(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int y1 = 0; y1 < n; y1++)
	{
		for(int y2 = 0; y2 < n; y2++)
		{
			if(y1 == y2)
			{
				continue;
			}
			if(boardPtr->rowFirstBoard[y1] == boardPtr->rowFirstBoard[y2])
			{
				return 0;
			}
		}
	}
	return 1;
}

uint8 passNoSameCols(Board *boardPtr)
{
	uint8 n = boardPtr->size;
	for(int x1 = 0; x1 < n; x1++)
	{
		for(int x2 = 0; x2 < n; x2++)
		{
			if(x1 == x2)
			{
				continue;
			}
			if(boardPtr->colFirstBoard[x1] == boardPtr->colFirstBoard[x2])
			{
				return 0;
			}
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

int checkAllConstraints(Board *boardPtr)
{
	uint8 n = boardPtr->size;
}

int main()
{
	Board b = createBoard(4);
	b.rowFirstBoard[0] = 0b0000000000000000000000000000000000000000000000000000000000000000;
	b.rowFirstBoard[1] = 0b0000000000000000000000000000000000000000000000000000000000000000;
	b.rowFirstBoard[2] = 0b0000000000000000000000000000000000000000000000000000000000000000;
	b.rowFirstBoard[3] = 0b0000000000000000000000000000000000000000000000000000000000000000;
	
	setPosition(&b, 0, 0, 0b11);
	setPosition(&b, 1, 0, 0b11);
	setPosition(&b, 2, 0, 0b10);
	setPosition(&b, 3, 0, 0b11);
	setPosition(&b, 0, 1, 0b11);
	setPosition(&b, 0, 2, 0b11);
	printf("%d\n", passRowCountConstraint(&b, 0));
	printf("%d\n", passColCountConstraint(&b, 0));
	printf("%d\n", passNoSameRows(&b));
	printf("%d\n", passNoSameCols(&b));
	printf("%d\n", passNoRunsOf3Row(&b, 0));
	printf("%d\n", passNoRunsOf3Col(&b, 0));
	
	return 0;
}