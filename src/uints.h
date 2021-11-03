#ifndef _uints_h
#define _uints_h

#include <stdint.h>
typedef uint_fast8_t uint8;
typedef uint_fast64_t uint64;
uint64 uint64max;
uint64 uint64WithBit(uint64 value, uint8 fromLeft, uint8 bitNumber, uint8 setToValue);
uint8 uint64GetBit(uint64 value, uint8 fromLeft, uint8 bit);

#endif