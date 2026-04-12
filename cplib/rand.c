#include "rand.h"
#include "common.h"

static uint32_t seed = 1;


inline void cpSetRandomSeed(const uint32_t value) {
    seed = value;
}

uint32_t cpGetRandomValueRaw() {
    // Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
    // don't let seed be zero!
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

inline int cpGetRandomValue(const int min, const int max) {
    return min + (int)(cpGetRandomValueRaw() % (uint32_t)(max - min));
}