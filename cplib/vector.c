#include "common.h"
#include "vector.h"

// TODO: maybe make more functions inline


#define F2_OP(op) return (cpVector2) { \
    fix16_##op(a.x, b.x), \
    fix16_##op(a.y, b.y), \
};
#define I2_OP(op) return (cpVector2i) { \
    (a.x) op (b.x), \
    (a.y) op (b.y), \
};
#define F2_OP_VAL(op) return (cpVector2) { \
    fix16_##op(a.x, b), \
    fix16_##op(a.y, b), \
};
#define I2_OP_VAL(op) return (cpVector2i) { \
    (a.x) op (b), \
    (a.y) op (b), \
};

#define F3_OP(op) return (cpVector3) { \
    fix16_##op(a.x, b.x), \
    fix16_##op(a.y, b.y), \
    fix16_##op(a.z, b.z), \
};
#define I3_OP(op) return (cpVector3i) { \
    (a.x) op (b.x), \
    (a.y) op (b.y), \
    (a.z) op (b.z), \
};
#define F3_OP_VAL(op) return (cpVector3) { \
    fix16_##op(a.x, b), \
    fix16_##op(a.y, b), \
    fix16_##op(a.z, b), \
};
#define I3_OP_VAL(op) return (cpVector3i) { \
    (a.x) op (b), \
    (a.y) op (b), \
    (a.z) op (b), \
};

#define F4_OP(op) return (cpVector4) { \
    fix16_##op(a.x, b.x), \
    fix16_##op(a.y, b.y), \
    fix16_##op(a.z, b.z), \
    fix16_##op(a.w, b.w), \
};
#define F4_OP_VAL(op) return (cpVector4) { \
    fix16_##op(a.x, b), \
    fix16_##op(a.y, b), \
    fix16_##op(a.z, b), \
    fix16_##op(a.w, b), \
};


/* cpVector2 */

cpVector2 cpVector2Add(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    F2_OP(add)
}

cpVector2 cpVector2AddValue(const cpVector2 a, const fix16_t b) [[unsequenced]] {
    F2_OP_VAL(add)
}

cpVector2 cpVector2Subtract(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    F2_OP(sub)
}

cpVector2 cpVector2SubtractValue(const cpVector2 a, const fix16_t b) [[unsequenced]] {
    F2_OP_VAL(sub)
}

cpVector2 cpVector2Multiply(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    F2_OP(mul)
}

cpVector2 cpVector2MultiplyValue(const cpVector2 a, const fix16_t b) [[unsequenced]] {
    F2_OP_VAL(mul)
}

cpVector2 cpVector2Divide(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    F2_OP(div)
}

cpVector2 cpVector2DivideValue(const cpVector2 a, const fix16_t b) [[unsequenced]] {
    F2_OP_VAL(div)
}

inline cpVector2 cpVector2Negate(const cpVector2 v) [[unsequenced]] {
    return cpVector2MultiplyValue(v, fix16_from_int(-1));
}

fix16_t cpVector2Length(const cpVector2 v) [[unsequenced]] {
    return fix16_sqrt(
        fix16_add(
            fix16_mul(v.x, v.x),
            fix16_mul(v.y, v.y)
        )
    );
}

fix16_t cpVector2Distance(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    const cpVector2 d = cpVector2Subtract(a, b);
    return cpVector2Length(d);
}

fix16_t cpVector2DotProduct(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    return fix16_add(
        fix16_mul(a.x, b.x),
        fix16_mul(a.y, b.y)
    );
}

fix16_t cpVector2CrossProduct(const cpVector2 a, const cpVector2 b) [[unsequenced]] {
    return fix16_sub(
        fix16_mul(a.x, b.y),
        fix16_mul(a.y, b.x)
    );
}

cpVector2 cpVector2Normalize(const cpVector2 v) [[unsequenced]] {
    const fix16_t length = cpVector2Length(v);
    return cpVector2DivideValue(
        v,
        length
    );
}

/* cpVector2i */

cpVector2i cpVector2iAdd(const cpVector2i a, const cpVector2i b) [[unsequenced]] {
    I2_OP(+)
}

cpVector2i cpVector2iAddValue(const cpVector2i a, const int b) [[unsequenced]] {
    I2_OP_VAL(+)
}

cpVector2i cpVector2iSubtract(const cpVector2i a, const cpVector2i b) [[unsequenced]] {
    I2_OP(-)
}

cpVector2i cpVector2iSubtractValue(const cpVector2i a, const int b) [[unsequenced]] {
    I2_OP_VAL(-)
}

cpVector2i cpVector2iMultiply(const cpVector2i a, const cpVector2i b) [[unsequenced]] {
    I2_OP(*)
}

cpVector2i cpVector2iMultiplyValue(const cpVector2i a, const int b) [[unsequenced]] {
    I2_OP_VAL(*)
}

cpVector2i cpVector2iDivide(const cpVector2i a, const cpVector2i b) [[unsequenced]] {
    I2_OP(/)
}

cpVector2i cpVector2iDivideValue(const cpVector2i a, const int b) [[unsequenced]] {
    I2_OP_VAL(/)
}

cpVector2i cpVector2iNegate(const cpVector2i v) [[unsequenced]] {
    return (cpVector2i) {
        -v.x,
        -v.y
    };
}

/* cpVector3 */

cpVector3 cpVector3Add(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    F3_OP(add)
}

cpVector3 cpVector3AddValue(const cpVector3 a, const fix16_t b) [[unsequenced]] {
    F3_OP_VAL(add)
}

cpVector3 cpVector3Subtract(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    F3_OP(sub)
}

cpVector3 cpVector3SubtractValue(const cpVector3 a, const fix16_t b) [[unsequenced]] {
    F3_OP_VAL(sub)
}

cpVector3 cpVector3Multiply(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    F3_OP(mul)
}

cpVector3 cpVector3MultiplyValue(const cpVector3 a, const fix16_t b) [[unsequenced]] {
    F3_OP_VAL(mul)
}

cpVector3 cpVector3Divide(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    F3_OP(div)
}

cpVector3 cpVector3DivideValue(const cpVector3 a, const fix16_t b) [[unsequenced]] {
    F3_OP_VAL(div)
}

inline cpVector3 cpVector3Negate(const cpVector3 v) [[unsequenced]] {
    return cpVector3MultiplyValue(v, fix16_from_int(-1));
}

fix16_t cpVector3Length(const cpVector3 v) [[unsequenced]] {
    return fix16_sqrt(
    fix16_add(
            fix16_mul(v.x, v.x),
            fix16_add(
                fix16_mul(v.y, v.y),
                fix16_mul(v.z, v.z)
            )
        )
    );
}

fix16_t cpVector3Distance(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    const cpVector3 d = cpVector3Subtract(a, b);
    return cpVector3Length(d);
}

fix16_t cpVector3DotProduct(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    return fix16_add(
        fix16_mul(a.x, b.x),
        fix16_add(
            fix16_mul(a.y, b.y),
            fix16_mul(a.z, b.z)
        )
    );
}

cpVector3 cpVector3CrossProduct(const cpVector3 a, const cpVector3 b) [[unsequenced]] {
    return (cpVector3) {
        fix16_sub(
            fix16_mul(a.y, b.z),
            fix16_mul(a.z, b.y)
        ),
        fix16_sub(
            fix16_mul(a.z, b.x),
            fix16_mul(a.x, b.z)
        ),
        fix16_sub(
            fix16_mul(a.x, b.y),
            fix16_mul(a.y, b.x)
        )
    };
}

cpVector3 cpVector3Normalize(const cpVector3 v) [[unsequenced]] {
    const fix16_t length = cpVector3Length(v);
    return cpVector3DivideValue(
        v,
        length
    );
}

/* cpVector3i */
#ifdef CPLIB_IMPLEMENT_VECTOR3I

cpVector3i cpVector3iAdd(const cpVector3i a, const cpVector3i b) [[unsequenced]] {
    I3_OP(+)
}

cpVector3i cpVector3iAddValue(const cpVector3i a, const int b) [[unsequenced]] {
    I3_OP_VAL(+)
}

cpVector3i cpVector3iSubtract(const cpVector3i a, const cpVector3i b) [[unsequenced]] {
    I3_OP(-)
}

cpVector3i cpVector3iSubtractValue(const cpVector3i a, const int b) [[unsequenced]] {
    I3_OP_VAL(-)
}

cpVector3i cpVector3iMultiply(const cpVector3i a, const cpVector3i b) [[unsequenced]] {
    I3_OP(*)
}

cpVector3i cpVector3iMultiplyValue(const cpVector3i a, const int b) [[unsequenced]] {
    I3_OP_VAL(*)
}

cpVector3i cpVector3iDivide(const cpVector3i a, const cpVector3i b) [[unsequenced]] {
    I3_OP(/)
}

cpVector3i cpVector3iDivideValue(const cpVector3i a, const int b) [[unsequenced]] {
    I3_OP_VAL(/)
}

cpVector3i cpVector3iNegate(const cpVector3i v) [[unsequenced]] {
    return (cpVector3i) {
        -v.x,
        -v.y,
        -v.z
    };
}

#endif

/* cpVector4 */
#ifdef CPLIB_IMPLEMENT_VECTOR4

cpVector4 cpVector4Add(const cpVector4 a, const cpVector4 b) [[unsequenced]] {
    F4_OP(add)
}

cpVector4 cpVector4AddValue(const cpVector4 a, const fix16_t b) [[unsequenced]] {
    F4_OP_VAL(add)
}

cpVector4 cpVector4Subtract(const cpVector4 a, const cpVector4 b) [[unsequenced]] {
    F4_OP(sub)
}

cpVector4 cpVector4SubtractValue(const cpVector4 a, const fix16_t b) [[unsequenced]] {
    F4_OP_VAL(sub)
}

cpVector4 cpVector4Multiply(const cpVector4 a, const cpVector4 b) [[unsequenced]] {
    F4_OP(mul)
}

cpVector4 cpVector4MultiplyValue(const cpVector4 a, const fix16_t b) [[unsequenced]] {
    F4_OP_VAL(mul)
}

cpVector4 cpVector4Divide(const cpVector4 a, const cpVector4 b) [[unsequenced]] {
    F4_OP(div)
}

cpVector4 cpVector4DivideValue(const cpVector4 a, const fix16_t b) [[unsequenced]] {
    F4_OP_VAL(div)
}

inline cpVector4 cpVector4Negate(const cpVector4 v) [[unsequenced]] {
    return cpVector4MultiplyValue(v, fix16_from_int(-1));
}

fix16_t cpVector4Length(const cpVector4 v) [[unsequenced]] {
    return fix16_sqrt(
        fix16_add(
            fix16_mul(v.x, v.x),
            fix16_add(
                fix16_mul(v.y, v.y),
                fix16_add(
                    fix16_mul(v.z, v.z),
                    fix16_mul(v.w, v.w)
                )
            )
        )
    );
}

fix16_t cpVector4Distance(const cpVector4 a, const cpVector4 b) [[unsequenced]] {
    const cpVector4 d = cpVector4Subtract(a, b);
    return cpVector4Length(d);
}

fix16_t cpVector4DotProduct(const cpVector4 a, const cpVector4 b) [[unsequenced]] {
    return fix16_add(
        fix16_mul(a.x, b.x),
        fix16_add(
            fix16_mul(a.y, b.y),
            fix16_add(
                fix16_mul(a.z, b.z),
                fix16_mul(a.w, a.w)
            )
        )
    );
}

// CrossProduct not implemented

cpVector4 cpVector4Normalize(const cpVector4 v) [[unsequenced]] {
    const fix16_t length = cpVector4Length(v);
    return cpVector4DivideValue(
        v,
        length
    );
}

#endif