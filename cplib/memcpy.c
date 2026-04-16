#include "memcpy.h"


#ifndef TARGET_PC
void* memcpy(void* destination, const void* source, size_t num) {
    uint8_t* dest = destination;
    const uint8_t* src = source;

    for (size_t i = 0; i < num; i++) {
        dest[i] = src[i];
    }

    return destination;
}
#endif