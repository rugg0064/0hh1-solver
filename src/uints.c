#include "uints.h"

uint64 uint64max = 0b1111111111111111111111111111111111111111111111111111111111111111;

uint8 uint64GetBit(uint64 value, uint8 fromLeft, uint8 bit)
{
	uint8 realBitValue = bit;
	if(fromLeft)
	{
		realBitValue = 63 - realBitValue;
	}
	uint64 mask = ((uint64)1) << realBitValue;
	uint64 result = (value & mask) >> realBitValue;
	return !result;
}

uint64 uint64WithBit(uint64 value, uint8 fromLeft, uint8 bitNumber, uint8 setToValue)
{
	uint8 bitOffset;
	if(fromLeft)
	{
		bitOffset = (63) - bitNumber;
	}
	else
	{
		bitOffset = bitNumber;
	}
	
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