#include<stdio.h>
#include "../uints.h"

void uint64WithBitTest()
{
	uint64 comparator = 0b1;
	for(int i = 0; i < 64; i++)
	{
		uint64 testSubject = uint64WithBit(0, 0, i, 1);
		uint8 equalsBitShift = comparator == testSubject;
		if(!equalsBitShift)
		{
			printf("Failed test 1%d:\n\t%lu\n\t%lu\n", i, comparator, testSubject);
		}
		comparator <<= 1;
	}
	
	//Starts from 0b1000..... 0b0100000 and does the same as above but starting from the left.
	comparator = (uint64)1 << 63;
	for(int i = 0; i < 64; i++)
	{
		uint64 testSubject = uint64WithBit(0, 1, i, 1);
		uint8 equalsBitShift = comparator == testSubject;
		if(!equalsBitShift)
		{
			printf("Failed test 2%d:\n\t%lu\n\t%lu\n", i, comparator, testSubject);
		}
		comparator >>= 1;
	}
}

void uint64GetBitTest()
{
	uint64 test1 = 0b1010101010101010101010101010101010101010101010101010101010101010;
	int expected1[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
	int expected2[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
	for(int i = 0; i < 64; i++)
	{
		if(expected1[i] != uint64GetBit(test1, 0, i))
		{
			printf("uint64GetbitTest failed at right i=%d\n", i);
		}
		if(expected2[i] != uint64GetBit(test1, 1, i))
		{
			printf("uint64GetbitTest failed at left i=%d\n", i);
		}
	}
}

int main()
{
	//Counts up from 0b1 0b10 0b100 0b1000 and compares to setting the nth bit to the same
	uint64WithBitTest();
	if(uint64max != 0b1111111111111111111111111111111111111111111111111111111111111111)
	{
		printf("uint64max was not the correct value\n");
	}
	uint64GetBitTest();
	
	
	return 0;
}