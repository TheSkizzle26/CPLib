#include "common.h"
#include "vector.h"

// TODO: replace with macros since we're just repeating the
// TODO: same things over and over again


/* cpVector3 */

cpVector3 cpVector3Add(const cpVector3 a, const cpVector3 b) {
    return (cpVector3) {
        fix16_add(a.x, b.x),
        fix16_add(a.y, b.y),
        fix16_add(a.z, b.z),
    };
}

cpVector3 cpVector3AddValue(const cpVector3 a, const fix16_t b) {
    return (cpVector3) {
        fix16_add(a.x, b),
        fix16_add(a.y, b),
        fix16_add(a.z, b),
    };
}

cpVector3 cpVector3Subtract(const cpVector3 a, const cpVector3 b) {
    return (cpVector3) {
        fix16_sub(a.x, b.x),
        fix16_sub(a.y, b.y),
        fix16_sub(a.z, b.z),
    };
}

cpVector3 cpVector3SubtractValue(const cpVector3 a, const fix16_t b) {
    return (cpVector3) {
        fix16_sub(a.x, b),
        fix16_sub(a.y, b),
        fix16_sub(a.z, b),
    };
}

cpVector3 cpVector3Multiply(const cpVector3 a, const cpVector3 b) {
    return (cpVector3) {
        fix16_mul(a.x, b.x),
        fix16_mul(a.y, b.y),
        fix16_mul(a.z, b.z),
    };
}

cpVector3 cpVector3MultiplyValue(const cpVector3 a, const fix16_t b) {
    return (cpVector3) {
        fix16_mul(a.x, b),
        fix16_mul(a.y, b),
        fix16_mul(a.z, b),
    };
}

cpVector3 cpVector3Divide(const cpVector3 a, const cpVector3 b) {
    return (cpVector3) {
        fix16_div(a.x, b.x),
        fix16_div(a.y, b.y),
        fix16_div(a.z, b.z),
    };
}

cpVector3 cpVector3DivideValue(const cpVector3 a, const fix16_t b) {
    return (cpVector3) {
        fix16_div(a.x, b),
        fix16_div(a.y, b),
        fix16_div(a.z, b),
    };
}

cpVector3 cpVector3Negate(cpVector3 v) {
    return (cpVector3) {
        -v.x,
        -v.y,
        -v.z
    };
}

fix16_t cpVector3Length(cpVector3 v) {
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

fix16_t cpVector3Distance(cpVector3 a, cpVector3 b) {
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

fix16_t cpVector3DotProduct(cpVector3 a, cpVector3 b) {
    return fix16_add(
        fix16_mul(a.x, b.x),
        fix16_add(
            fix16_mul(a.y, b.y),
            fix16_mul(a.z, b.z)
        )
    );
}

cpVector3 cpVector3CrossProduct(cpVector3 a, cpVector3 b) {
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

cpVector3 cpVector3Normalize(cpVector3 v) {
    const fix16_t length = cpVector3Length(v);
    return cpVector3DivideValue(
        v,
        length
    );
}

/* cpVector3i */

cpVector3i cpVector3iAdd(cpVector3 a, cpVector3 b) {
    return (cpVector3i) {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}

cpVector3i cpVector3iAddValue(cpVector3 a, fix16_t b) {
    return (cpVector3i) {
        a.x + b,
        a.y + b,
        a.z + b,
    };
}

cpVector3i cpVector3iSubtract(cpVector3 a, cpVector3 b) {
    return (cpVector3i) {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
}

cpVector3i cpVector3iSubtractValue(cpVector3 a, fix16_t b) {
    return (cpVector3i) {
        a.x - b,
        a.y - b,
        a.z - b,
    };
}

cpVector3i cpVector3iMultiply(cpVector3 a, cpVector3 b) {
    return (cpVector3i) {
        a.x * b.x,
        a.y * b.y,
        a.z * b.z,
    };
}

cpVector3i cpVector3iMultiplyValue(cpVector3 a, fix16_t b) {
    return (cpVector3i) {
        a.x * b,
        a.y * b,
        a.z * b,
    };
}

cpVector3i cpVector3iDivide(cpVector3 a, cpVector3 b) {
    return (cpVector3i) {
        a.x / b.x,
        a.y / b.y,
        a.z / b.z,
    };
}

cpVector3i cpVector3iDivideValue(cpVector3 a, fix16_t b) {
    return (cpVector3i) {
        a.x / b,
        a.y / b,
        a.z / b,
    };
}

cpVector3i cpVector3iNegate(cpVector3i v) {
    return (cpVector3i) {
        -v.x,
        -v.y,
        -v.z
    };
}


