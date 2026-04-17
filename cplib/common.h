#ifndef CPLIB_COMMON_H
#define CPLIB_COMMON_H


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "libfixmath/fixmath.h"

typedef uint16_t cpColor;


#ifdef TARGET_PC

#include <stdlib.h>
#include <string.h>

#else

void free(void* ptr);
void* malloc(size_t size);
void* memcpy(void* destination, const void* source, size_t num);
void* memset(void* ptr, int value, size_t num);

char *strcat(char *dest, const char *src);
const char *strchr(const char *str, char c);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *destination, const char *source);
int strlen(const char *str);

#endif


#endif