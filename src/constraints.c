#include "constraints.h"

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