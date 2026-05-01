#include "rand.h"
#include "common.h"

uint32_t cpInternalRandomSeed = 1;


void cpSetRandomSeed(uint32_t value);

uint32_t cpGetRandomValueRaw() {
    // Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
    // don't let seed be zero!
    cpInternalRandomSeed ^= cpInternalRandomSeed << 13;
    cpInternalRandomSeed ^= cpInternalRandomSeed >> 17;
    cpInternalRandomSeed ^= cpInternalRandomSeed << 5;
    return cpInternalRandomSeed;
}

int cpGetRandomValue(int min, int max);