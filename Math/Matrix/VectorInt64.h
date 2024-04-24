/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef MATH_MATRIX_VECTORFLOAT32_H
#define MATH_MATRIX_VECTORFLOAT32_H

#include "Types.h"
#include <immintrin.h>
#include <xmmintrin.h>

struct simd_int64_2 {
    union {
        __m128i P;
        int64 v[2];
    };
};

struct simd_int64_4 {
    union {
        __m256i P;
        int64 v[4];
    };
};

struct simd_int64_8 {
    union {
        __m512i P;
        int64 v[8];
    };
};

struct v3_simd_int64_2 {
    union {
        struct {
            union {
                simd_int64_2 x;
                simd_int64_2 r;
            };
            union {
                simd_int64_2 y;
                simd_int64_2 g;
            };
            union {
                simd_int64_2 z;
                simd_int64_2 b;
            };
        };

        simd_int64_2 v[3];
    };
};

struct v3_simd_int64_4 {
    union {
        struct {
            union {
                simd_int64_4 x;
                simd_int64_4 r;
            };
            union {
                simd_int64_4 y;
                simd_int64_4 g;
            };
            union {
                simd_int64_4 z;
                simd_int64_4 b;
            };
        };

        simd_int64_4 v[3];
    };
};

struct v3_simd_int64_8 {
    union {
        struct {
            union {
                simd_int64_8 x;
                simd_int64_8 r;
            };
            union {
                simd_int64_8 y;
                simd_int64_8 g;
            };
            union {
                simd_int64_8 z;
                simd_int64_8 b;
            };
        };

        simd_int64_8 v[3];
    };
};

struct v4_simd_int64_2 {
    union {
        struct {
            union {
                simd_int64_2 x;
                simd_int64_2 r;
            };
            union {
                simd_int64_2 y;
                simd_int64_2 g;
            };
            union {
                simd_int64_2 z;
                simd_int64_2 b;
            };
            union {
                simd_int64_2 w;
                simd_int64_2 a;
            };
        };

        simd_int64_2 v[4];
    };
};

struct v4_simd_int64_4 {
    union {
        struct {
            union {
                simd_int64_4 x;
                simd_int64_4 r;
            };
            union {
                simd_int64_4 y;
                simd_int64_4 g;
            };
            union {
                simd_int64_4 z;
                simd_int64_4 b;
            };
            union {
                simd_int64_4 w;
                simd_int64_4 a;
            };
        };

        simd_int64_4 v[4];
    };
};

struct v4_simd_int64_8 {
    union {
        struct {
            union {
                simd_int64_8 x;
                simd_int64_8 r;
            };
            union {
                simd_int64_8 y;
                simd_int64_8 g;
            };
            union {
                simd_int64_8 z;
                simd_int64_8 b;
            };
            union {
                simd_int64_8 w;
                simd_int64_8 a;
            };
        };

        simd_int64_8 v[4];
    };
};

#endif
