#include "common.h"
#include "vector.h"
#include "matrix.h"


cpMatrix3 cpMatrix3Transpose(cpMatrix3 A) {
    return (cpMatrix3) {
        A.xx, A.yx, A.zx,
        A.xy, A.yy, A.zy,
        A.xz, A.yz, A.zz,
    };
}

cpMatrix3 cpMatrix3Multiply(const cpMatrix3 A, const cpMatrix3 B) {
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

cpVector3 cpMatrix3MultiplyVector(const cpMatrix3 A, const cpVector3 v) {
    return (cpVector3) {
        fix16_add(fix16_add(fix16_mul(A.xx, v.x),  fix16_mul(A.xy, v.y)), fix16_mul(A.xz, v.z)),
        fix16_add(fix16_add(fix16_mul(A.yx, v.x),  fix16_mul(A.yy, v.y)), fix16_mul(A.yz, v.z)),
        fix16_add(fix16_add(fix16_mul(A.zx, v.x),  fix16_mul(A.zy, v.y)), fix16_mul(A.zz, v.z)),
    };
}

cpMatrix4 cpMatrix4Multiply(const cpMatrix4 A, const cpMatrix4 B) {
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

cpVector4 cpMatrix4MultiplyVector(const cpMatrix4 A, const cpVector4 v) {
    return (cpVector4) {
        fix16_add(fix16_add(fix16_add(fix16_mul(A.xx, v.x), fix16_mul(A.xy, v.y)), fix16_mul(A.xz, v.z)), fix16_mul(A.xw, v.w)),
        fix16_add(fix16_add(fix16_add(fix16_mul(A.yx, v.x), fix16_mul(A.yy, v.y)), fix16_mul(A.yz, v.z)), fix16_mul(A.yw, v.w)),
        fix16_add(fix16_add(fix16_add(fix16_mul(A.zx, v.x), fix16_mul(A.zy, v.y)), fix16_mul(A.zz, v.z)), fix16_mul(A.zw, v.w)),
        fix16_add(fix16_add(fix16_add(fix16_mul(A.wx, v.x), fix16_mul(A.wy, v.y)), fix16_mul(A.wz, v.z)), fix16_mul(A.ww, v.w)),
    };
}
