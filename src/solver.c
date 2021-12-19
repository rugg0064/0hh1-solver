#include "solver.h"

#include "constraints.h"

/*
int fixSandwiches(Board *boardPtr)
{ //A sandwich is a case where it is X_X (both X are the same color)
  //Due to the run of 3 rule, the center must be the opposite value.
  //Returns true if something changed
	int changed = 0;
	uint8 n = boardPtr->size;
	for(int y = 0; y < n; y++)
	{
		for(int x = 0; x < n-2; x++)
		{
			if(!isSlotEmpty(boardPtr, x, y) && !isSlotEmpty(boardPtr, x+2, y))
			{
				uint8 value = getColor(boardPtr, x, y);
				if( value == getColor(boardPtr, x+2, y))
				{
					if(isSlotEmpty(boardPtr, x+1, y))
					{
						setColor(boardPtr, x+1, y, 0b10 + (value ^ 1)); //Set the value to 0b10 (not empty) and the opposite value.
						changed = 1;
					}
				}
			}
		}
	}
	
	for(int x = 0; x < n; x++)
	{
		for(int y = 0; y < n-2; y++)
		{
			if(!isSlotEmpty(boardPtr, x, y) && !isSlotEmpty(boardPtr, x, y+2))
			{
				uint8 value = getColor(boardPtr, x, y);
				if(value == getColor(boardPtr, x, y+2))
				{
					if(isSlotEmpty(boardPtr, x, y+1))
					{
						setColor(boardPtr, x, y+1, 0b10 + (value ^ 1)); //Set the value to 0b10 (not empty) and the opposite value.
						changed = 1;
					}
				}
			}
		}
	}
	return changed;
}

int fixRunsOfTwo(Board *boardPtr)
{
	//returns true if the function changed something
	int changed = 0;
	uint8 n = boardPtr->size;
	for(int y = 0; y < n; y++)
	{
		for(int x = 0; x < n-1; x++)
		{
			if(!isSlotEmpty(boardPtr, x, y) && !isSlotEmpty(boardPtr, x+1, y))
			{
				uint8 value = getColor(boardPtr, x, y);
				if(value == getColor(boardPtr, x+1, y))
				{
					if(x>=1)
					{
						if(isSlotEmpty(boardPtr, x-1, y))
						{
							setColor(boardPtr, x-1, y, 0b10 + (value ^ 1)); //Set the value to 0b10 (not empty) and the opposite value.
							changed = 1;
						}
					}
					if(x<=n-3)
					{
						if(isSlotEmpty(boardPtr, x+2, y))
						{
							setColor(boardPtr, x+2, y, 0b10 + (value ^ 1)); //Set the value to 0b10 (not empty) and the opposite value.
							changed = 1;
						}
					}
				}
			}
		}
	}
	
	for(int x = 0; x < n; x++)
	{
		for(int y = 0; y < n-1; y++)
		{
			if(!isSlotEmpty(boardPtr, x, y) && !isSlotEmpty(boardPtr, x, y+1))
			{
				uint8 value = getColor(boardPtr, x, y);
				if(value == getColor(boardPtr, x, y+1))
				{
					if(y>=1)
					{
						if(isSlotEmpty(boardPtr, x, y-1))
						{
							setColor(boardPtr, x, y-1, 0b10 + (value ^ 1)); //Set the value to 0b10 (not empty) and the opposite value.
							changed = 1;
						}
					}
					if(y<=n-3)
					{
						if(isSlotEmpty(boardPtr, x, y+2))
						{
							setColor(boardPtr, x, y+2, 0b10 + (value ^ 1)); //Set the value to 0b10 (not empty) and the opposite value.
							changed = 1;
						}
					}
				}
			}
		}
	}
	return changed;
}


int fix2n(Board *boardPtr)
{
	int changed = 0;
	uint8 n = boardPtr->size;
	uint8 limit = n/2;

	for(int y = 0; y < n; y++)
	{
		uint8 redCount = 0;
		uint8 blueCount = 0;
		for(int x = 0; x < n; x++)
		{
			//Uses a similar trick from when we were constraint checking
			if(!isSlotEmpty(boardPtr, x, y))
			{
				uint8 value = getColor(boardPtr, x, y);
				redCount  += value ^ 1;
				blueCount += value;
			}
		}
		if((redCount >= limit) || (blueCount >= limit))
		{
			uint8 valueToSet = redCount >= limit ? 1 : 0;
			for(int x = 0; x < n; x++)
			{
				if(isSlotEmpty(boardPtr, x, y))
				{
					setColor(boardPtr, x, y, 0b10 + valueToSet);
					changed = 1;
				}
			}
		}
	}
	
	for(int x = 0; x < n; x++)
	{
		uint8 redCount = 0;
		uint8 blueCount = 0;
		for(int y = 0; y < n; y++)
		{
			//Uses a similar trick from when we were constraint checking
			if(!isSlotEmpty(boardPtr, x, y))
			{
				uint8 value = getColor(boardPtr, x, y);
				redCount  += value ^ 1;
				blueCount += value;
			}
		}
		if((redCount >= limit) || (blueCount >= limit))
		{
			uint8 valueToSet = redCount >= limit ? 1 : 0;
			for(int y = 0; y < n; y++)
			{
				if(isSlotEmpty(boardPtr, x, y))
				{
					setColor(boardPtr, x, y, 0b10 + valueToSet);
					changed = 1;
				}
			}
		}
	}
	return changed;
}
*/

void fixLine(Board *boardPtr, uint8 isCol, uint8 lineNum)
{
	uint8 n = boardPtr->size;
	uint8 redCount;
	uint8 blueCount;
	
	/* Find the red and blue count on the line */
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
	uint8 shouldFill = 0;
	uint8 fillColor = 0;
	if(redCount == limit)
	{
		shouldFill = 1;
		fillColor = 0;
	}
	else if(redCount == limit)
	{
		shouldFill = 1;
		fillColor = 0;
	}
	if(shouldFill)
	{
		for(int i = 0; i < n; i++)
		{
			if(isCol)
			{
				setFilled(boardPtr, lineNum, i, 1);
				setColor(boardPtr, lineNum, i, fillColor);
			}
			else
			{
				setFilled(boardPtr, i, lineNum, 1);
				setColor(boardPtr, i, lineNum, fillColor);
			}
			//Store all positions that got changed in some data structure
			//Then at the end of fixing all the things
			//recurse onto each spot.
		}
	}
}

Board solve(Board board)
{
	//printf("Recursed\n");
	//printBoard(&board);
	//printf("\n");
	if(!isBoardValid(&board, 0b0111111))
	{
		printf("Invalid, skipping");
		return board;
	}
	if(isBoardFull(&board))
	{
		//printf("asd\n");
		return board;
	}
	int changed = 1;
	while(changed)
	{
		changed = 0;
		//changed = fixSandwiches(&board);
		//changed |= fixRunsOfTwo(&board);
		//changed |= fix2n(&board);
		printBoard(&board);
		//printf("\n");
	}
	return board;
	//Since we are returning a board object, we cant return NULL or something
	//to tell people that our solution was invalid, so we have to check that
	//after returning.
	int n = board.size;
	for(int y = 0; y < n; y++)
	{
		for(int x = 0; x < n; x++)
		{
			Board newBoard = board; //Make a copy of the current board
			if(isSlotEmpty(&newBoard, x, y))
			{
				setColor(&newBoard, x, y, 0b10);
				//if(canInsert(&newBoard, x, y, 0))
				{
					newBoard = solve(newBoard);
					if(isBoardValid(&newBoard, 0b1111111))
					{
						return newBoard;
					}
				}
				//if(canInsert(&newBoard, x, y, 1))
				{
					setColor(&newBoard, x, y, 0b11);
					newBoard = solve(newBoard);
					if(isBoardValid(&newBoard, 0b1111111))
					{
						return newBoard;
					}
				}
				
			}
		}
	}
	return board;
}