#include "common.h"
#include "vector.h"
#include "matrix.h"


cpMatrix3 cpMatrix3CreateIdentity() [[unsequenced]] {
    return (cpMatrix3) {
        fix16_one,  0,          0,
        0,          fix16_one,  0,
        0,          0,          fix16_one,
    };
}

cpMatrix3 cpMatrix3CreateScalar(const fix16_t value) [[unsequenced]] {
    return (cpMatrix3) {
        value,      0,          0,
        0,          value,  0,
        0,          0,      value,
    };
}

cpMatrix3 cpMatrix3CreateRotation(const cpVector3 direction) [[unsequenced]] {
    const cpVector3 right = cpVector3Normalize(cpVector3CrossProduct((cpVector3) {0, fix16_one, 0}, direction));
    const cpVector3 up = cpVector3CrossProduct(direction, right);

    return (cpMatrix3) {
        right.x, up.x, direction.x,
        right.y, up.y, direction.y,
        right.z, up.z, direction.z,
    };
}

cpMatrix3 cpMatrix3Transpose(const cpMatrix3 A) [[unsequenced]] {
    return (cpMatrix3) {
        A.xx, A.yx, A.zx,
        A.xy, A.yy, A.zy,
        A.xz, A.yz, A.zz,
    };
}

cpMatrix3 cpMatrix3Multiply(const cpMatrix3 A, const cpMatrix3 B) [[unsequenced]] {
    cpMatrix3 C = {0};

    C.xx = fix16_add(fix16_add(fix16_mul(A.xx, B.xx), fix16_mul(A.xy, B.yx)), fix16_mul(A.xz, B.zx));
    C.xy = fix16_add(fix16_add(fix16_mul(A.xx, B.xy), fix16_mul(A.xy, B.yy)), fix16_mul(A.xz, B.zy));
    C.xz = fix16_add(fix16_add(fix16_mul(A.xx, B.xz), fix16_mul(A.xy, B.yz)), fix16_mul(A.xz, B.zz));

    C.yx = fix16_add(fix16_add(fix16_mul(A.yx, B.xx), fix16_mul(A.yy, B.yx)), fix16_mul(A.yz, B.zx));
    C.yy = fix16_add(fix16_add(fix16_mul(A.yx, B.xy), fix16_mul(A.yy, B.yy)), fix16_mul(A.yz, B.zy));
    C.yz = fix16_add(fix16_add(fix16_mul(A.yx, B.xz), fix16_mul(A.yy, B.yz)), fix16_mul(A.yz, B.zz));

    C.zx = fix16_add(fix16_add(fix16_mul(A.zx, B.xx), fix16_mul(A.zy, B.yx)), fix16_mul(A.zz, B.zx));
    C.zy = fix16_add(fix16_add(fix16_mul(A.zx, B.xy), fix16_mul(A.zy, B.yy)), fix16_mul(A.zz, B.zy));
    C.zz = fix16_add(fix16_add(fix16_mul(A.zx, B.xz), fix16_mul(A.zy, B.yz)), fix16_mul(A.zz, B.zz));

    return C;
}

cpVector3 cpMatrix3MultiplyVector(const cpMatrix3 A, const cpVector3 v) [[unsequenced]] {
    return (cpVector3) {
        fix16_add(fix16_add(fix16_mul(A.xx, v.x),  fix16_mul(A.xy, v.y)), fix16_mul(A.xz, v.z)),
        fix16_add(fix16_add(fix16_mul(A.yx, v.x),  fix16_mul(A.yy, v.y)), fix16_mul(A.yz, v.z)),
        fix16_add(fix16_add(fix16_mul(A.zx, v.x),  fix16_mul(A.zy, v.y)), fix16_mul(A.zz, v.z)),
    };
}

#ifdef CPLIB_IMPLEMENT_MATRIX4

cpMatrix4 cpMatrix4CreateIdentity() [[unsequenced]] {
    return (cpMatrix4) {
        fix16_one,  0,          0,          0,
        0,          fix16_one,  0,          0,
        0,          0,          fix16_one,  0,
        0,          0,          0,          fix16_one,
    };
}

cpMatrix4 cpMatrix4CreateScalar(const fix16_t value) [[unsequenced]] {
    return (cpMatrix4) {
        value,      0,          0,          0,
        0,          value,      0,          0,
        0,          0,          value,      0,
        0,          0,          0,          value,
    };
}

cpMatrix4 cpMatrix4CreateRotation(const cpVector3 direction) [[unsequenced]] {
    const cpVector3 right = cpVector3Normalize(cpVector3CrossProduct((cpVector3) {0, fix16_one, 0}, direction));
    const cpVector3 up = cpVector3CrossProduct(direction, right);

    return (cpMatrix4) {
        right.x, up.x, direction.x, 0,
        right.y, up.y, direction.y, 0,
        right.z, up.z, direction.z, 0,
        0, 0, 0, fix16_one,
    };
}

cpMatrix4 cpMatrix4Transpose(const cpMatrix4 A) [[unsequenced]] {
    return (cpMatrix4) {
        A.xx, A.yx, A.zx, A.wx,
        A.xy, A.yy, A.zy, A.wy,
        A.xz, A.yz, A.zz, A.wz,
        A.xw, A.yw, A.zw, A.ww,
    };
}

cpMatrix4 cpMatrix4Multiply(const cpMatrix4 A, const cpMatrix4 B) [[unsequenced]] {
    cpMatrix4 C = {0};

    // wtf...
    // I don't think this is very efficient...

    C.xx = fix16_add(fix16_add(fix16_add(fix16_mul(A.xx, B.xx), fix16_mul(A.xy, B.yx)), fix16_mul(A.xz, B.zx)), fix16_mul(A.xw, B.wx));
    C.xy = fix16_add(fix16_add(fix16_add(fix16_mul(A.xx, B.xy), fix16_mul(A.xy, B.yy)), fix16_mul(A.xz, B.zy)), fix16_mul(A.xw, B.wy));
    C.xz = fix16_add(fix16_add(fix16_add(fix16_mul(A.xx, B.xz), fix16_mul(A.xy, B.yz)), fix16_mul(A.xz, B.zz)), fix16_mul(A.xw, B.wz));
    C.xw = fix16_add(fix16_add(fix16_add(fix16_mul(A.xx, B.xw), fix16_mul(A.xy, B.yw)), fix16_mul(A.xz, B.zw)), fix16_mul(A.xw, B.ww));

    C.yx = fix16_add(fix16_add(fix16_add(fix16_mul(A.yx, B.xx), fix16_mul(A.yy, B.yx)), fix16_mul(A.yz, B.zx)), fix16_mul(A.yw, B.wx));
    C.yy = fix16_add(fix16_add(fix16_add(fix16_mul(A.yx, B.xy), fix16_mul(A.yy, B.yy)), fix16_mul(A.yz, B.zy)), fix16_mul(A.yw, B.wy));
    C.yz = fix16_add(fix16_add(fix16_add(fix16_mul(A.yx, B.xz), fix16_mul(A.yy, B.yz)), fix16_mul(A.yz, B.zz)), fix16_mul(A.yw, B.wz));
    C.yw = fix16_add(fix16_add(fix16_add(fix16_mul(A.yx, B.xw), fix16_mul(A.yy, B.yw)), fix16_mul(A.yz, B.zw)), fix16_mul(A.yw, B.ww));

    C.zx = fix16_add(fix16_add(fix16_add(fix16_mul(A.zx, B.xx), fix16_mul(A.zy, B.yx)), fix16_mul(A.zz, B.zx)), fix16_mul(A.zw, B.wx));
    C.zy = fix16_add(fix16_add(fix16_add(fix16_mul(A.zx, B.xy), fix16_mul(A.zy, B.yy)), fix16_mul(A.zz, B.zy)), fix16_mul(A.zw, B.wy));
    C.zz = fix16_add(fix16_add(fix16_add(fix16_mul(A.zx, B.xz), fix16_mul(A.zy, B.yz)), fix16_mul(A.zz, B.zz)), fix16_mul(A.zw, B.wz));
    C.zw = fix16_add(fix16_add(fix16_add(fix16_mul(A.zx, B.xw), fix16_mul(A.zy, B.yw)), fix16_mul(A.zz, B.zw)), fix16_mul(A.zw, B.ww));

    C.wx = fix16_add(fix16_add(fix16_add(fix16_mul(A.wx, B.xx), fix16_mul(A.wy, B.yx)), fix16_mul(A.wz, B.zx)), fix16_mul(A.ww, B.wx));
    C.wy = fix16_add(fix16_add(fix16_add(fix16_mul(A.wx, B.xy), fix16_mul(A.wy, B.yy)), fix16_mul(A.wz, B.zy)), fix16_mul(A.ww, B.wy));
    C.wz = fix16_add(fix16_add(fix16_add(fix16_mul(A.wx, B.xz), fix16_mul(A.wy, B.yz)), fix16_mul(A.wz, B.zz)), fix16_mul(A.ww, B.wz));
    C.ww = fix16_add(fix16_add(fix16_add(fix16_mul(A.wx, B.xw), fix16_mul(A.wy, B.yw)), fix16_mul(A.wz, B.zw)), fix16_mul(A.ww, B.ww));

    return C;
}

cpVector4 cpMatrix4MultiplyVector(const cpMatrix4 A, const cpVector4 v) [[unsequenced]] {
    return (cpVector4) {
        fix16_add(fix16_add(fix16_add(fix16_mul(A.xx, v.x), fix16_mul(A.xy, v.y)), fix16_mul(A.xz, v.z)), fix16_mul(A.xw, v.w)),
        fix16_add(fix16_add(fix16_add(fix16_mul(A.yx, v.x), fix16_mul(A.yy, v.y)), fix16_mul(A.yz, v.z)), fix16_mul(A.yw, v.w)),
        fix16_add(fix16_add(fix16_add(fix16_mul(A.zx, v.x), fix16_mul(A.zy, v.y)), fix16_mul(A.zz, v.z)), fix16_mul(A.zw, v.w)),
        fix16_add(fix16_add(fix16_add(fix16_mul(A.wx, v.x), fix16_mul(A.wy, v.y)), fix16_mul(A.wz, v.z)), fix16_mul(A.ww, v.w)),
    };
}

#endif