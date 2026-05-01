#ifndef CPLIB_RAND_H
#define CPLIB_RAND_H

#include "common.h"

extern uint32_t cpInternalRandomSeed;


// Set the RNG's seed. Must not be zero.
static inline void cpSetRandomSeed(const uint32_t value) {
    if (value)
        cpInternalRandomSeed = value;
}

uint32_t cpGetRandomValueRaw(); // Get a raw random value between 0 and UINT32_MAX-1.

// Get a random value between min and max.
static inline int cpGetRandomValue(const int min, const int max) {
    return min + (int)(cpGetRandomValueRaw() % (uint32_t)(max - min));
}


#endif