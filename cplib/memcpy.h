#ifndef CPLIB_MEMCPY_H
#define CPLIB_MEMCPY_H

#include "common.h"

#ifndef TARGET_PC
// the os memcpy crashes for some reason?!?
void* memcpy(void* destination, const void* source, size_t num);
#endif

#endif