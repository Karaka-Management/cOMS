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

#include "../../../stdlib/Types.h"
#include "SIMD_F64.h"

struct int64_2 {
    union {
        #if ARM
            svint64_t s;
        #else
            __m128i s;
        #endif

        int64 v[2];
    };
};

struct int64_4 {
    union {
        #if ARM
            svint64_t s;
        #else
            __m256i s;
        #endif

        int64 v[4];
    };
};

struct int64_8 {
    union {
        #if ARM
            svint64_t s;
        #else
            __m512i s;
        #endif

        int64 v[8];
    };
};

#endif