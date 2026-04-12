/*
 * Way too many functions for vector arithmetic.
 */

#ifndef CPLIB_VECTOR_H
#define CPLIB_VECTOR_H

#include "common.h"


typedef struct {
    fix16_t x, y;
} cpVector2;

typedef struct {
    int x, y;
} cpVector2i;

typedef struct {
    fix16_t x, y, z;
} cpVector3;

typedef struct {
    int x, y, z;
} cpVector3i;


// I was bored.

cpVector2 cpVector2Add(cpVector2 a, cpVector2 b); // uses software floats!
cpVector2 cpVector2AddValue(cpVector2 a, fix16_t b); // uses software floats!
cpVector2 cpVector2Subtract(cpVector2 a, cpVector2 b); // uses software floats!
cpVector2 cpVector2SubtractValue(cpVector2 a, fix16_t b); // uses software floats!
cpVector2 cpVector2Multiply(cpVector2 a, cpVector2 b); // uses software floats!
cpVector2 cpVector2MultiplyValue(cpVector2 a, fix16_t b); // uses software floats!
cpVector2 cpVector2Divide(cpVector2 a, cpVector2 b); // uses software floats!
cpVector2 cpVector2DivideValue(cpVector2 a, fix16_t b); // uses software floats!

cpVector2 cpVector2Negate(cpVector2 v); // uses software floats!
fix16_t cpVector2Length(cpVector2 v); // uses software floats!
fix16_t cpVector2Distance(cpVector2 a, cpVector2 b); // uses software floats!
fix16_t cpVector2DotProduct(cpVector2 a, cpVector2 b); // uses software floats!
fix16_t cpVector2CrossProduct(cpVector2 a, cpVector2 b); // uses software floats!
cpVector2 cpVector2Normalize(cpVector2 v); // uses software floats!

cpVector2i cpVector2iAdd(cpVector2i a, cpVector2i b);
cpVector2i cpVector2iAddValue(cpVector2i a, int b);
cpVector2i cpVector2iSubtract(cpVector2i a, cpVector2i b);
cpVector2i cpVector2iSubtractValue(cpVector2i a, int b);
cpVector2i cpVector2iMultiply(cpVector2i a, cpVector2i b);
cpVector2i cpVector2iMultiplyValue(cpVector2i a, int b);
cpVector2i cpVector2iDivide(cpVector2i a, cpVector2i b);
cpVector2i cpVector2iDivideValue(cpVector2i a, int b);

cpVector2i cpVector2iNegate(cpVector2i v);

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

cpVector3i cpVector3iAdd(cpVector3i a, cpVector3i b);
cpVector3i cpVector3iAddValue(cpVector3i a, int b);
cpVector3i cpVector3iSubtract(cpVector3i a, cpVector3i b);
cpVector3i cpVector3iSubtractValue(cpVector3i a, int b);
cpVector3i cpVector3iMultiply(cpVector3i a, cpVector3i b);
cpVector3i cpVector3iMultiplyValue(cpVector3i a, int b);
cpVector3i cpVector3iDivide(cpVector3i a, cpVector3i b);
cpVector3i cpVector3iDivideValue(cpVector3i a, int b);

cpVector3i cpVector3iNegate(cpVector3i v);


#endif