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
        #if ARM
            svfloat64_t s;
        #else
            __m128 s;
        #endif

        f64 v[2];
    };
};

struct f64_4 {
    union {
        #if ARM
            svfloat64_t s;
        #else
            __m256 s;
        #endif

        f64 v[4];
    };
};

struct f64_8 {
    union {
        #if ARM
            svfloat64_t s;
        #else
            __m512 s;
        #endif

        f64 v[8];
    };
};

#endif