#ifndef CPLIB_RAND_H
#define CPLIB_RAND_H


#include "common.h"

void cpSetRandomSeed(uint32_t value);
uint32_t cpGetRandomValueRaw();
int cpGetRandomValue(int min, int max);


#endif