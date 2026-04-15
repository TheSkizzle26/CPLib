#ifndef CPLIB_MATRIX_H
#define CPLIB_MATRIX_H

#include "common.h"
#include "vector.h"


typedef struct {
    fix16_t xx, xy, xz;
    fix16_t yx, yy, yz;
    fix16_t zx, zy, zz;
} cpMatrix3;

#ifdef CPLIB_IMPLEMENT_MATRIX4
typedef struct {
    fix16_t xx, xy, xz, xw;
    fix16_t yx, yy, yz, yw;
    fix16_t zx, zy, zz, zw;
    fix16_t wx, wy, wz, ww;
} cpMatrix4;
#endif

// Not really used anywhere, but though I should add them anyway.
// I probably won't add any other operations

cpMatrix3 cpMatrix3Transpose(cpMatrix3 A);
cpMatrix3 cpMatrix3Multiply(cpMatrix3 A, cpMatrix3 B);
cpVector3 cpMatrix3MultiplyVector(cpMatrix3 A, cpVector3 v);

#ifdef CPLIB_IMPLEMENT_MATRIX4
cpMatrix4 cpMatrix4Transpose(cpMatrix4 A);
cpMatrix4 cpMatrix4Multiply(cpMatrix4 A, cpMatrix4 B);
cpVector4 cpMatrix4MultiplyVector(cpMatrix4 A, cpVector4 v);
#endif

#endif