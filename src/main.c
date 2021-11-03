#include <stdio.h>
#include <string.h>
#include "board.h"
#include "uints.h"
#include "solver.h"

int main()
{
	Board b;
	
	FILE *filePointer = fopen("./input.txt", "r");
	int lineNumber = 0;
	int size;
	int first = 1;
	char line[32];
	while(!feof(filePointer))
	{
		fgets(line, 32, filePointer);
		if(first) 
		{
			first = 0;
			size = strlen(line)-2;
			b = createBoard(size);
		}
		
		for(int i = 0; i < size; i++)
		{
			uint8 value = 0;
			switch(line[i]) 
			{
				case 'R':
					value = 0b10;
					break;
				case 'B':
					value = 0b11;
					break;
			}
			setValue(&b, i, lineNumber, value);
		}
		
		lineNumber++;
	}
	fclose(filePointer);
	printBoard(&b);
	printf("\n");
	if(isBoardValid(&b, 0b0111111))
	{
		Board result = solve(b);
		printBoard(&result);
		printf("Is it valid: %d\n", isBoardValid(&result, 0b1111111));
	}
	else
	{
		printf("Starting board was invalid");
	}
	return 0;
}