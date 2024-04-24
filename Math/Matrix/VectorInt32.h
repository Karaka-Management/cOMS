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

struct simd_int32_4 {
    union {
        __m128i P;
        int32 v[4];
    };
};

struct simd_int32_8 {
    union {
        __m256i P;
        int32 v[8];
    };
};

struct simd_int32_16 {
    union {
        __m512i P;
        int32 v[16];
    };
};

struct v3_simd_int32_4 {
    union {
        struct {
            union {
                simd_int32_4 x;
                simd_int32_4 r;
            };
            union {
                simd_int32_4 y;
                simd_int32_4 g;
            };
            union {
                simd_int32_4 z;
                simd_int32_4 b;
            };
        };

        simd_int32_4 v[3];
    };
};

struct v3_simd_int32_8 {
    union {
        struct {
            union {
                simd_int32_8 x;
                simd_int32_8 r;
            };
            union {
                simd_int32_8 y;
                simd_int32_8 g;
            };
            union {
                simd_int32_8 z;
                simd_int32_8 b;
            };
        };

        simd_int32_8 v[3];
    };
};

struct v3_simd_int32_16 {
    union {
        struct {
            union {
                simd_int32_16 x;
                simd_int32_16 r;
            };
            union {
                simd_int32_16 y;
                simd_int32_16 g;
            };
            union {
                simd_int32_16 z;
                simd_int32_16 b;
            };
        };

        simd_int32_16 v[3];
    };
};

struct v4_simd_int32_4 {
    union {
        struct {
            union {
                simd_int32_4 x;
                simd_int32_4 r;
            };
            union {
                simd_int32_4 y;
                simd_int32_4 g;
            };
            union {
                simd_int32_4 z;
                simd_int32_4 b;
            };
            union {
                simd_int32_4 w;
                simd_int32_4 a;
            };
        };

        simd_int32_4 v[4];
    };
};

struct v4_simd_int32_8 {
    union {
        struct {
            union {
                simd_int32_8 x;
                simd_int32_8 r;
            };
            union {
                simd_int32_8 y;
                simd_int32_8 g;
            };
            union {
                simd_int32_8 z;
                simd_int32_8 b;
            };
            union {
                simd_int32_8 w;
                simd_int32_8 a;
            };
        };

        simd_int32_8 v[4];
    };
};

struct v4_simd_int32_16 {
    union {
        struct {
            union {
                simd_int32_16 x;
                simd_int32_16 r;
            };
            union {
                simd_int32_16 y;
                simd_int32_16 g;
            };
            union {
                simd_int32_16 z;
                simd_int32_16 b;
            };
            union {
                simd_int32_16 w;
                simd_int32_16 a;
            };
        };

        simd_int32_16 v[4];
    };
};

#endif
