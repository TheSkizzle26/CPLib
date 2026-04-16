#ifndef CPLIB_MATRIX_H
#define CPLIB_MATRIX_H

#include "common.h"
#include "vector.h"


typedef struct {
    fix16_t xx, xy, xz;
    fix16_t yx, yy, yz;
    fix16_t zx, zy, zz;
} cpMatrix3;

#ifdef CPLIB_ENABLE_MATRIX4
typedef struct {
    fix16_t xx, xy, xz, xw;
    fix16_t yx, yy, yz, yw;
    fix16_t zx, zy, zz, zw;
    fix16_t wx, wy, wz, ww;
} cpMatrix4;
#endif


// TODO: add more operations

cpMatrix3 cpMatrix3CreateIdentity() [[unsequenced]];
cpMatrix3 cpMatrix3CreateScalar(fix16_t value) [[unsequenced]];
cpMatrix3 cpMatrix3CreateRotation(cpVector3 direction) [[unsequenced]];
cpMatrix3 cpMatrix3Transpose(cpMatrix3 A) [[unsequenced]];
cpMatrix3 cpMatrix3Multiply(cpMatrix3 A, cpMatrix3 B) [[unsequenced]];
cpVector3 cpMatrix3MultiplyVector(cpMatrix3 A, cpVector3 v) [[unsequenced]];

#ifdef CPLIB_ENABLE_MATRIX4
cpMatrix4 cpMatrix4CreateIdentity() [[unsequenced]];
cpMatrix4 cpMatrix3CreateScalar(fix16_t value) [[unsequenced]];
cpMatrix4 cpMatrix4CreateRotation(cpVector4 direction) [[unsequenced]];
cpMatrix4 cpMatrix4Transpose(cpMatrix4 A) [[unsequenced]];
cpMatrix4 cpMatrix4Multiply(cpMatrix4 A, cpMatrix4 B) [[unsequenced]];
cpVector4 cpMatrix4MultiplyVector(cpMatrix4 A, cpVector4 v) [[unsequenced]];
#endif

#endif