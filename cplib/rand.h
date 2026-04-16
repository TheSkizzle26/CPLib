#ifndef CPLIB_RAND_H
#define CPLIB_RAND_H


#include "common.h"

void cpSetRandomSeed(uint32_t value); // Set the RNG's seed. Must not be zero.
uint32_t cpGetRandomValueRaw(); // Get a raw random value between 0 and UINT32_MAX-1.
int cpGetRandomValue(int min, int max); // Get a random value between min and max.


#endif