#include "common.h"
#include "vector.h"

// TODO: replace with macros since we're just repeating the
// TODO: same things over and over again
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


/* cpVector3 */

cpVector3 cpVector3Add(const cpVector3 a, const cpVector3 b) {
    F3_OP(add)
}

cpVector3 cpVector3AddValue(const cpVector3 a, const fix16_t b) {
    F3_OP_VAL(add)
}

cpVector3 cpVector3Subtract(const cpVector3 a, const cpVector3 b) {
    F3_OP(sub)
}

cpVector3 cpVector3SubtractValue(const cpVector3 a, const fix16_t b) {
    F3_OP_VAL(sub)
}

cpVector3 cpVector3Multiply(const cpVector3 a, const cpVector3 b) {
    F3_OP(mul)
}

cpVector3 cpVector3MultiplyValue(const cpVector3 a, const fix16_t b) {
    F3_OP_VAL(mul)
}

cpVector3 cpVector3Divide(const cpVector3 a, const cpVector3 b) {
    F3_OP(div)
}

cpVector3 cpVector3DivideValue(const cpVector3 a, const fix16_t b) {
    F3_OP_VAL(div)
}

inline cpVector3 cpVector3Negate(const cpVector3 v) {
    return cpVector3MultiplyValue(v, fix16_from_int(-1));
}

fix16_t cpVector3Length(const cpVector3 v) {
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

fix16_t cpVector3Distance(const cpVector3 a, const cpVector3 b) {
    const cpVector3 d = cpVector3Subtract(a, b);

    return fix16_sqrt(
    fix16_add(
            fix16_mul(d.x, d.x),
            fix16_add(
                fix16_mul(d.y, d.y),
                fix16_mul(d.z, d.z)
            )
        )
    );
}

fix16_t cpVector3DotProduct(const cpVector3 a, const cpVector3 b) {
    return fix16_add(
        fix16_mul(a.x, b.x),
        fix16_add(
            fix16_mul(a.y, b.y),
            fix16_mul(a.z, b.z)
        )
    );
}

cpVector3 cpVector3CrossProduct(const cpVector3 a, const cpVector3 b) {
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

cpVector3 cpVector3Normalize(const cpVector3 v) {
    const fix16_t length = cpVector3Length(v);
    return cpVector3DivideValue(
        v,
        length
    );
}

/* cpVector3i */

cpVector3i cpVector3iAdd(const cpVector3i a, const cpVector3i b) {
    I3_OP(+)
}

cpVector3i cpVector3iAddValue(const cpVector3i a, const int b) {
    I3_OP_VAL(+)
}

cpVector3i cpVector3iSubtract(const cpVector3i a, const cpVector3i b) {
    I3_OP(-)
}

cpVector3i cpVector3iSubtractValue(const cpVector3i a, const int b) {
    I3_OP_VAL(-)
}

cpVector3i cpVector3iMultiply(const cpVector3i a, const cpVector3i b) {
    I3_OP(*)
}

cpVector3i cpVector3iMultiplyValue(const cpVector3i a, const int b) {
    I3_OP_VAL(*)
}

cpVector3i cpVector3iDivide(const cpVector3i a, const cpVector3i b) {
    I3_OP(/)
}

cpVector3i cpVector3iDivideValue(const cpVector3i a, const int b) {
    I3_OP_VAL(/)
}

cpVector3i cpVector3iNegate(const cpVector3i v) {
    return (cpVector3i) {
        -v.x,
        -v.y,
        -v.z
    };
}


