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
#include <xmmintrin.h>
#include <immintrin.h>

struct simd_f32_4 {
    union {
        __m128 P;
        f32 v[4];
    };
};

struct simd_f32_8 {
    union {
        __m256 P;
        f32 v[8];
    };
};

struct simd_f32_16 {
    union {
        __m512 P;
        f32 v[16];
    };
};

inline
simd_f32_4 init_zero_simd_f32_4()
{
    simd_f32_4 simd;
    simd.P = _mm_setzero_ps();

    return simd;
}

inline
simd_f32_8 init_zero_simd_f32_8()
{
    simd_f32_8 simd;
    simd.P = _mm256_setzero_ps();

    return simd;
}

inline
simd_f32_16 init_zero_simd_f32_16()
{
    simd_f32_16 simd;
    simd.P = _mm512_setzero_ps();

    return simd;
}

inline
simd_f32_4 operator+(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_add_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator+(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_add_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator+(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_add_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 operator-(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_sub_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 operator-(simd_f32_4 a)
{
    return init_zero_simd_f32_4() - a;
}

inline
simd_f32_8 operator-(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_sub_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator-(simd_f32_8 a)
{
    return init_zero_simd_f32_8() - a;
}

inline
simd_f32_16 operator-(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_sub_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator-(simd_f32_16 a)
{
    return init_zero_simd_f32_16() - a;
}

inline
simd_f32_4 operator*(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_mul_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator*(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_mul_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator*(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mul_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 operator/(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_div_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator/(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_div_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator/(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_div_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 operator^(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_xor_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator^(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_xor_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator^(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_xor_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4& operator-=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a - b;

    return a;
}

inline
simd_f32_8& operator-=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a - b;

    return a;
}

inline
simd_f32_16& operator-=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a - b;

    return a;
}

inline
simd_f32_4& operator+=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a + b;

    return a;
}

inline
simd_f32_8& operator+=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a + b;

    return a;
}

inline
simd_f32_16& operator+=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a + b;

    return a;
}

inline
simd_f32_4& operator*=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a * b;

    return a;
}

inline
simd_f32_8& operator*=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a * b;

    return a;
}

inline
simd_f32_16& operator*=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a * b;

    return a;
}

inline
simd_f32_4& operator/=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a / b;

    return a;
}

inline
simd_f32_8& operator/=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a / b;

    return a;
}

inline
simd_f32_16& operator/=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a / b;

    return a;
}

inline
simd_f32_4& operator^=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a ^ b;

    return a;
}

inline
simd_f32_8& operator^=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a ^ b;

    return a;
}

inline
simd_f32_16& operator^=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a ^ b;

    return a;
}

inline
simd_f32_4 operator<(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_cmplt_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator<(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_cmp_ps(a.P, b.P, _CMP_LT_OQ);

    return simd;
}

inline
simd_f32_16 operator<(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mask_mov_ps(
        _mm512_setzero_ps(),
        _mm512_cmp_ps_mask(a.P, b.P, _CMP_LT_OQ),
        _mm512_set1_ps(1.0f)
    );

    return simd;
}

inline
simd_f32_4 operator<=(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_cmple_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator<=(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_cmp_ps(a.P, b.P, _CMP_LE_OQ);

    return simd;
}

inline
simd_f32_16 operator<=(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mask_mov_ps(
        _mm512_setzero_ps(),
        _mm512_cmp_ps_mask(a.P, b.P, _CMP_LE_OQ),
        _mm512_set1_ps(1.0f)
    );

    return simd;
}

inline
simd_f32_4 operator>(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_cmpgt_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator>(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_cmp_ps(a.P, b.P, _CMP_GT_OQ);

    return simd;
}

inline
simd_f32_16 operator>(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mask_mov_ps(
        _mm512_setzero_ps(),
        _mm512_cmp_ps_mask(a.P, b.P, _CMP_GT_OQ),
        _mm512_set1_ps(1.0f)
    );

    return simd;
}

inline
simd_f32_4 operator>=(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_cmpge_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator>=(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_cmp_ps(a.P, b.P, _CMP_GE_OQ);

    return simd;
}

inline
simd_f32_16 operator>=(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mask_mov_ps(
        _mm512_setzero_ps(),
        _mm512_cmp_ps_mask(a.P, b.P, _CMP_GE_OQ),
        _mm512_set1_ps(1.0f)
    );

    return simd;
}

inline
simd_f32_4 operator==(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_cmpeq_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator==(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_cmp_ps(a.P, b.P, _CMP_EQ_OQ);

    return simd;
}

inline
simd_f32_16 operator==(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mask_mov_ps(
        _mm512_setzero_ps(),
        _mm512_cmp_ps_mask(a.P, b.P, _CMP_EQ_OQ),
        _mm512_set1_ps(1.0f)
    );

    return simd;
}

inline
simd_f32_4 operator!=(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_cmpneq_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator!=(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_cmp_ps(a.P, b.P, _CMP_NEQ_OQ);

    return simd;
}

inline
simd_f32_16 operator!=(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_mask_mov_ps(
        _mm512_setzero_ps(),
        _mm512_cmp_ps_mask(a.P, b.P, _CMP_NEQ_OQ),
        _mm512_set1_ps(1.0f)
    );

    return simd;
}

inline
simd_f32_4 operator&(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_and_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator&(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_and_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator&(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_and_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 operator|(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_or_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 operator|(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_or_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 operator|(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_or_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4& operator&=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a & b;

    return a;
}

inline
simd_f32_8& operator&=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a & b;

    return a;
}

inline
simd_f32_16& operator&=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a & b;

    return a;
}

inline
simd_f32_4& operator|=(simd_f32_4 &a, simd_f32_4 b)
{
    a = a | b;

    return a;
}

inline
simd_f32_8& operator|=(simd_f32_8 &a, simd_f32_8 b)
{
    a = a | b;

    return a;
}

inline
simd_f32_16& operator|=(simd_f32_16 &a, simd_f32_16 b)
{
    a = a | b;

    return a;
}

inline
simd_f32_4 abs(simd_f32_4 a)
{
    unsigned int unsigned_mask = (unsigned int) (1 << 31);
    __m128 mask = _mm_set1_ps(*(float *) &unsigned_mask);

    simd_f32_4 simd;
    simd.P = _mm_and_ps(a.P, mask);

    return simd;
}

inline
simd_f32_8 abs(simd_f32_8 a)
{
    unsigned int unsigned_mask = (unsigned int) (1 << 31);
    __m256 mask = _mm256_set1_ps(*(float *) &unsigned_mask);

    simd_f32_8 simd;
    simd.P = _mm256_and_ps(a.P, mask);

    return simd;
}

inline
simd_f32_16 abs(simd_f32_16 a)
{
    unsigned int unsigned_mask = (unsigned int) (1 << 31);
    __m512 mask = _mm512_set1_ps(*(float *) &unsigned_mask);

    simd_f32_16 simd;
    simd.P = _mm512_and_ps(a.P, mask);

    return simd;
}

inline
simd_f32_4 min(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_min_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 min(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_min_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 min(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_min_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 max(simd_f32_4 a, simd_f32_4 b)
{
    simd_f32_4 simd;
    simd.P = _mm_max_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_8 max(simd_f32_8 a, simd_f32_8 b)
{
    simd_f32_8 simd;
    simd.P = _mm256_max_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_16 max(simd_f32_16 a, simd_f32_16 b)
{
    simd_f32_16 simd;
    simd.P = _mm512_max_ps(a.P, b.P);

    return simd;
}

inline
simd_f32_4 sign(simd_f32_4 a)
{
    unsigned int umask = (unsigned int) (1 << 31);
    __m128 mask = _mm_set1_ps(*(float *) &umask);

    simd_f32_4 signBit;
    signBit.P = _mm_and_ps(a.P, mask);

    simd_f32_4 b;
    b.P = _mm_set1_ps(1.0f);

    simd_f32_4 simd = b | signBit;

    return simd;
}

inline
simd_f32_8 sign(simd_f32_8 a)
{
    unsigned int umask = (unsigned int) (1 << 31);
    __m256 mask = _mm256_set1_ps(*(float *) &umask);

    simd_f32_8 signBit;
    signBit.P = _mm256_and_ps(a.P, mask);

    simd_f32_8 b;
    b.P = _mm256_set1_ps(1.0f);

    simd_f32_8 simd = b | signBit;

    return simd;
}

inline
simd_f32_16 sign(simd_f32_16 a)
{
    unsigned int umask = (unsigned int) (1 << 31);
    __m512 mask = _mm512_set1_ps(*(float *) &umask);

    simd_f32_16 signBit;
    signBit.P = _mm512_and_ps(a.P, mask);

    simd_f32_16 b;
    b.P = _mm512_set1_ps(1.0f);

    simd_f32_16 simd = b | signBit;

    return simd;
}

// sqrt
// approxinvsquareroot
// approx1over
// clamp
// floor
// ceil
// anytrue
// alltrue
// anyfalse
// allfalse

struct v3_simd_f32_4 {
    union {
        struct {
            union {
                simd_f32_4 x;
                simd_f32_4 r;
            };
            union {
                simd_f32_4 y;
                simd_f32_4 g;
            };
            union {
                simd_f32_4 z;
                simd_f32_4 b;
            };
        };

        simd_f32_4 v[3];
    };
};

struct v3_simd_f32_8 {
    union {
        struct {
            union {
                simd_f32_8 x;
                simd_f32_8 r;
            };
            union {
                simd_f32_8 y;
                simd_f32_8 g;
            };
            union {
                simd_f32_8 z;
                simd_f32_8 b;
            };
        };

        simd_f32_8 v[3];
    };
};

struct v3_simd_f32_16 {
    union {
        struct {
            union {
                simd_f32_16 x;
                simd_f32_16 r;
            };
            union {
                simd_f32_16 y;
                simd_f32_16 g;
            };
            union {
                simd_f32_16 z;
                simd_f32_16 b;
            };
        };

        simd_f32_16 v[3];
    };
};

struct v4_simd_f32_4 {
    union {
        struct {
            union {
                simd_f32_4 x;
                simd_f32_4 r;
            };
            union {
                simd_f32_4 y;
                simd_f32_4 g;
            };
            union {
                simd_f32_4 z;
                simd_f32_4 b;
            };
            union {
                simd_f32_4 w;
                simd_f32_4 a;
            };
        };

        simd_f32_4 v[4];
    };
};

struct v4_simd_f32_8 {
    union {
        struct {
            union {
                simd_f32_8 x;
                simd_f32_8 r;
            };
            union {
                simd_f32_8 y;
                simd_f32_8 g;
            };
            union {
                simd_f32_8 z;
                simd_f32_8 b;
            };
            union {
                simd_f32_8 w;
                simd_f32_8 a;
            };
        };

        simd_f32_8 v[4];
    };
};

struct v4_simd_f32_16 {
    union {
        struct {
            union {
                simd_f32_16 x;
                simd_f32_16 r;
            };
            union {
                simd_f32_16 y;
                simd_f32_16 g;
            };
            union {
                simd_f32_16 z;
                simd_f32_16 b;
            };
            union {
                simd_f32_16 w;
                simd_f32_16 a;
            };
        };

        simd_f32_16 v[4];
    };
};

#endif
