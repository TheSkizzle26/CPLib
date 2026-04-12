/*
 * Way too many functions for vector arithmetic.
 */

#ifndef CPLIB_VECTOR_H
#define CPLIB_VECTOR_H

#include "common.h"


typedef struct {
    fix16_t x, y, z;
} cpVector3;

typedef struct {
    int x, y, z;
} cpVector3i;


cpVector3 cpVector3Add(cpVector3 a, cpVector3 b); // uses software floats!
cpVector3 cpVector3AddValue(cpVector3 a, fix16_t b); // uses software floats!
cpVector3 cpVector3Subtract(cpVector3 a, cpVector3 b); // uses software floats!
cpVector3 cpVector3SubtractValue(cpVector3 a, fix16_t b); // uses software floats!
cpVector3 cpVector3Multiply(cpVector3 a, cpVector3 b); // uses software floats!
cpVector3 cpVector3MultiplyValue(cpVector3 a, fix16_t b); // uses software floats!
cpVector3 cpVector3Divide(cpVector3 a, cpVector3 b); // uses software floats!
cpVector3 cpVector3DivideValue(cpVector3 a, fix16_t b); // uses software floats!

cpVector3 cpVector3Negate(cpVector3 v); // uses software floats!
fix16_t cpVector3Length(cpVector3 v); // uses software floats!
fix16_t cpVector3Distance(cpVector3 a, cpVector3 b); // uses software floats!
fix16_t cpVector3DotProduct(cpVector3 a, cpVector3 b); // uses software floats!
cpVector3 cpVector3CrossProduct(cpVector3 a, cpVector3 b); // uses software floats!
cpVector3 cpVector3Normalize(cpVector3 v); // uses software floats!

cpVector3i cpVector3iAdd(cpVector3 a, cpVector3 b);
cpVector3i cpVector3iAddValue(cpVector3 a, fix16_t b);
cpVector3i cpVector3iSubtract(cpVector3 a, cpVector3 b);
cpVector3i cpVector3iSubtractValue(cpVector3 a, fix16_t b);
cpVector3i cpVector3iMultiply(cpVector3 a, cpVector3 b);
cpVector3i cpVector3iMultiplyValue(cpVector3 a, fix16_t b);
cpVector3i cpVector3iDivide(cpVector3 a, cpVector3 b);
cpVector3i cpVector3iDivideValue(cpVector3 a, fix16_t b);

cpVector3i cpVector3iNegate(cpVector3i v);


#endif