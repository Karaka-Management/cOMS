/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_SIMD_F64_H
#define TOS_STDLIB_SIMD_F64_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"

struct f64_2 {
    union {
        __m128 s;
        f64 v[2];
    };
};

struct f64_4 {
    union {
        __m256 s;
        f64 v[4];
    };
};

struct f64_8 {
    union {
        __m512 s;
        f64 v[8];
    };
};

#endif