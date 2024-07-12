/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_TOS_STDLIB_SIMD_I64_H
#define TOS_TOS_STDLIB_SIMD_I64_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"
#include "SIMD_F64.h"

struct int64_2 {
    union {
        __m128i s;
        int64 v[2];
    };
};

struct int64_4 {
    union {
        __m256i s;
        int64 v[4];
    };
};

struct int64_8 {
    union {
        __m512i s;
        int64 v[8];
    };
};

#endif