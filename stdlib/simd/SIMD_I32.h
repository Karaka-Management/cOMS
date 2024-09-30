/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_TOS_STDLIB_SIMD_I32_H
#define TOS_TOS_STDLIB_SIMD_I32_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"
#include "../../utils/BitUtils.h"
#include "SIMD_F32.h"

// @todo a lot of sse functions require high level (e.g. sse4.1) this needs to be changed to be more general
//      or better create alternative functions for the available sse version.

// @question why are we passing structs by value?

struct int32_4 {
    union {
        __m128i s;
        int32 v[4];
    };
};

struct int32_8 {
    union {
        __m256i s;
        int32 v[8];
    };
};

struct int32_16 {
    union {
        __m512i s;
        int32 v[16];
    };
};

inline int32_4 load_int32_4(const int32* mem)
{
    int32_4 simd;
    simd.s = _mm_loadu_epi32(mem);

    return simd;
}

inline int32_4 init_int32_4(const int32* mem)
{
    int32_4 simd;
    simd.s = _mm_set_epi32(mem[0], mem[1], mem[2], mem[3]);

    return simd;
}

inline void unload_int32_4(int32_4 a, int32 *array) { _mm_store_si128((__m128i *) array, a.s); }

inline int32_8 load_int32_8(const int32* mem)
{
    int32_8 simd;
    simd.s = _mm256_loadu_epi32(mem);

    return simd;
}

inline int32_8 init_int32_8(const int32* mem)
{
    int32_8 simd;
    simd.s = _mm256_set_epi32(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]);

    return simd;
}

inline void unload_int32_8(int32_8 a, int32 *array) { _mm256_store_si256((__m256i *) array, a.s); }

inline int32_16 load_int32_16(const int32* mem)
{
    int32_16 simd;
    simd.s = _mm512_loadu_epi32(mem);

    return simd;
}

inline int32_16 init_int32_16(const int32* mem)
{
    int32_16 simd;
    simd.s = _mm512_set_epi32(
        mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7],
        mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15]);

    return simd;
}

inline void unload_int32_16(int32_16 a, int32 *array) { _mm512_store_epi32(array, a.s); }

inline int32_4 init_zero_int32_4()
{
    int32_4 simd;
    simd.s = _mm_setzero_si128();

    return simd;
}

inline int32_8 init_zero_int32_8()
{
    int32_8 simd;
    simd.s = _mm256_setzero_si256();

    return simd;
}

inline int32_16 init_zero_int32_16()
{
    int32_16 simd;
    simd.s = _mm512_setzero_epi32();

    return simd;
}

inline int32_4 init_value_int32_4(int32 value)
{
    int32_4 simd;
    simd.s = _mm_set1_epi32(value);

    return simd;
}

inline int32_8 init_value_int32_8(int32 value)
{
    int32_8 simd;
    simd.s = _mm256_set1_epi32(value);

    return simd;
}

inline int32_16 init_value_int32_16(int32 value)
{
    int32_16 simd;
    simd.s = _mm512_set1_epi32(value);

    return simd;
}

inline int32_4 init_values_int32_4(int32 a, int32 b, int32 c, int32 d)
{
    int32_4 simd;
    simd.s = _mm_set_epi32(a, b, c, d);

    return simd;
}

inline int32_8 init_values_int32_8(
    int32 a, int32 b, int32 c, int32 d,
    int32 e, int32 f, int32 g, int32 h
)
{
    int32_8 simd;
    simd.s = _mm256_set_epi32(a, b, c, d, e, f, g, h);

    return simd;
}

inline int32_16 init_values_int32_16(
    int32 a, int32 b, int32 c, int32 d,
    int32 e, int32 f, int32 g, int32 h,
    int32 i, int32 j, int32 k, int32 l,
    int32 m, int32 n, int32 o, int32 p
)
{
    int32_16 simd;
    simd.s = _mm512_set_epi32(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);

    return simd;
}

inline
int32_4 f32_4_to_int32_4(f32_4 a)
{
    int32_4 result;
    result.s = _mm_cvtps_epi32(a.s);

    return result;
}

inline
f32_4 int32_4_to_f32_4(int32_4 a)
{
    f32_4 result;
    result.s = _mm_cvtepi32_ps(a.s);

    return result;
}

inline
int32_8 f32_8_to_int32_8(f32_8 a)
{
    int32_8 result;
    result.s = _mm256_cvtps_epi32(a.s);

    return result;
}

inline
f32_8 int32_8_to_f32_8(int32_8 a)
{
    f32_8 result;
    result.s = _mm256_cvtepi32_ps(a.s);

    return result;
}

inline
int32_16 f32_16_to_int32_16(f32_16 a)
{
    int32_16 result;
    result.s = _mm512_cvtps_epi32(a.s);

    return result;
}

inline
f32_16 int32_16_to_f32_16(int32_16 a)
{
    f32_16 result;
    result.s = _mm512_cvtepi32_ps(a.s);

    return result;
}

inline int32_4 operator+(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_add_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator+(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_add_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator+(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_add_epi32(a.s, b.s);

    return simd;
}

inline int32_4 operator-(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_sub_epi32(a.s, b.s);

    return simd;
}

inline int32_4 operator-(int32_4 a) { return init_zero_int32_4() - a; }

inline int32_8 operator-(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_sub_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator-(int32_8 a) { return init_zero_int32_8() - a; }

inline int32_16 operator-(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_sub_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator-(int32_16 a) { return init_zero_int32_16() - a; }

inline int32_4 operator*(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_mul_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator*(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_mul_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator*(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mul_epi32(a.s, b.s);

    return simd;
}

inline int32_4 operator/(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_div_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator/(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_div_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator/(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_div_epi32(a.s, b.s);

    return simd;
}

inline f32_4 operator/(f32_4 a, int32_4 b)
{
    f32_4 simd;
    simd.s = _mm_div_ps(a.s, _mm_cvtepi32_ps(b.s));

    return simd;
}

inline f32_8 operator/(f32_8 a, int32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_div_ps(a.s, _mm256_cvtepi32_ps(b.s));

    return simd;
}

inline f32_16 operator/(f32_16 a, int32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_div_ps(a.s, _mm512_cvtepi32_ps(b.s));

    return simd;
}

inline f32_4 operator/(int32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_div_ps(_mm_cvtepi32_ps(a.s), b.s);

    return simd;
}

inline f32_8 operator/(int32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_div_ps(_mm256_cvtepi32_ps(a.s), b.s);

    return simd;
}

inline f32_16 operator/(int32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_div_ps(_mm512_cvtepi32_ps(a.s), b.s);

    return simd;
}

inline int32_4 operator^(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_xor_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator^(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_xor_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator^(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_xor_epi32(a.s, b.s);

    return simd;
}

inline int32_4 &operator-=(int32_4 &a, int32_4 b)
{
    a = a - b;

    return a;
}

inline int32_8 &operator-=(int32_8 &a, int32_8 b)
{
    a = a - b;

    return a;
}

inline int32_16 &operator-=(int32_16 &a, int32_16 b)
{
    a = a - b;

    return a;
}

inline int32_4 &operator+=(int32_4 &a, int32_4 b)
{
    a = a + b;

    return a;
}

inline int32_8 &operator+=(int32_8 &a, int32_8 b)
{
    a = a + b;

    return a;
}

inline int32_16 &operator+=(int32_16 &a, int32_16 b)
{
    a = a + b;

    return a;
}

inline int32_4 &operator*=(int32_4 &a, int32_4 b)
{
    a = a * b;

    return a;
}

inline int32_8 &operator*=(int32_8 &a, int32_8 b)
{
    a = a * b;

    return a;
}

inline int32_16 &operator*=(int32_16 &a, int32_16 b)
{
    a = a * b;

    return a;
}

inline int32_4 &operator/=(int32_4 &a, int32_4 b)
{
    a.s = (a / b).s;

    return a;
}

inline int32_8 &operator/=(int32_8 &a, int32_8 b)
{
    a.s = (a / b).s;

    return a;
}

inline int32_16 &operator/=(int32_16 &a, int32_16 b)
{
    a.s = (a / b).s;

    return a;
}

inline int32_4 &operator^=(int32_4 &a, int32_4 b)
{
    a = a ^ b;

    return a;
}

inline int32_8 &operator^=(int32_8 &a, int32_8 b)
{
    a = a ^ b;

    return a;
}

inline int32_16 &operator^=(int32_16 &a, int32_16 b)
{
    a = a ^ b;

    return a;
}

inline int32_4 operator<(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_cmplt_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator<(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_xor_si256(_mm256_cmpgt_epi32(a.s, b.s), _mm256_set1_epi32(-1));

    return simd;
}

inline int32_16 operator<(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mask_blend_epi32(_mm512_cmplt_epi32_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int32_4 operator<=(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_andnot_si128(_mm_cmplt_epi32(b.s, a.s), _mm_set1_epi32(-1));

    return simd;
}

inline int32_8 operator<=(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi32(a.s, b.s), _mm256_set1_epi32(-1));

    return simd;
}

inline int32_16 operator<=(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mask_blend_epi32(_mm512_knot(_mm512_cmpgt_epi32_mask(b.s, a.s)), b.s, a.s);

    return simd;
}

inline int32_4 operator>(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_cmpgt_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator>(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_cmpgt_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator>(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mask_blend_epi32(_mm512_cmpgt_epi32_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int32_4 operator>=(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_andnot_si128(_mm_cmplt_epi32(a.s, b.s), _mm_set1_epi32(-1));

    return simd;
}

inline int32_8 operator>=(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi32(b.s, a.s), _mm256_set1_epi32(-1));

    return simd;
}

inline int32_16 operator>=(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mask_blend_epi32(_mm512_cmpge_epi32_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int32_4 operator==(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_cmpeq_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator==(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_cmpeq_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator==(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mask_blend_epi32(_mm512_cmpeq_epi32_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int32_4 operator!=(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_andnot_si128(_mm_cmpeq_epi32(a.s, b.s), _mm_set1_epi32(-1));

    return simd;
}

inline int32_8 operator!=(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_mask_blend_epi32(_mm256_cmp_epi32_mask(a.s, b.s, _MM_CMPINT_NE), a.s, b.s);

    return simd;
}

inline int32_16 operator!=(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_mask_blend_epi32(_mm512_cmp_epi32_mask(a.s, b.s, _MM_CMPINT_NE), a.s, b.s);

    return simd;
}

inline int32_4 operator&(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_and_si128(a.s, b.s);

    return simd;
}

inline int32_8 operator&(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_and_si256(a.s, b.s);

    return simd;
}

inline int32_16 operator&(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_and_si512(a.s, b.s);

    return simd;
}

inline int32_4 operator|(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_or_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator|(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_or_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator|(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_or_epi32(a.s, b.s);

    return simd;
}

inline int32_4 &operator&=(int32_4 &a, int32_4 b)
{
    a = a & b;

    return a;
}

inline int32_8 &operator&=(int32_8 &a, int32_8 b)
{
    a = a & b;

    return a;
}

inline int32_16 &operator&=(int32_16 &a, int32_16 b)
{
    a = a & b;

    return a;
}

inline int32_4 &operator|=(int32_4 &a, int32_4 b)
{
    a = a | b;

    return a;
}

inline int32_8 &operator|=(int32_8 &a, int32_8 b)
{
    a = a | b;

    return a;
}

inline int32_16 &operator|=(int32_16 &a, int32_16 b)
{
    a = a | b;

    return a;
}

inline int32_4 abs(int32_4 a)
{
    int32_4 simd;
    simd.s = _mm_abs_epi32(a.s);

    return simd;
}

inline int32_8 abs(int32_8 a)
{
    int32_8 simd;
    simd.s = _mm256_abs_epi32(a.s);

    return simd;
}

inline int32_16 abs(int32_16 a)
{
    int32_16 simd;
    simd.s = _mm512_abs_epi64(a.s);

    return simd;
}

inline int32_4 simd_min(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_min_epi32(a.s, b.s);

    return simd;
}

inline int32_8 simd_min(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_min_epi32(a.s, b.s);

    return simd;
}

inline int32_16 simd_min(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_min_epi32(a.s, b.s);

    return simd;
}

inline int32_4 simd_max(int32_4 a, int32_4 b)
{
    int32_4 simd;
    simd.s = _mm_max_epi32(a.s, b.s);

    return simd;
}

inline int32_8 simd_max(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_max_epi32(a.s, b.s);

    return simd;
}

inline int32_16 simd_max(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_max_epi32(a.s, b.s);

    return simd;
}

inline int32_4 sign(int32_4 a)
{
    __m128i mask = _mm_set1_epi32(0x80000000);
    __m128i signBit = _mm_and_si128(a.s, mask);
    __m128i b = _mm_set1_epi32(1);

    int32_4 simd;
    simd.s = _mm_or_si128(b, signBit);

    return simd;
}

inline int32_8 sign(int32_8 a)
{
    __m256i mask = _mm256_set1_epi32(0x80000000);
    __m256i signBit = _mm256_and_si256(a.s, mask);
    __m256i b = _mm256_set1_epi32(1);

    int32_8 simd;
    simd.s = _mm256_or_si256(b, signBit);

    return simd;
}

inline int32_16 sign(int32_16 a)
{
    __m512i mask = _mm512_set1_epi32(0x80000000);
    __m512i signBit = _mm512_and_si512(a.s, mask);
    __m512i b = _mm512_set1_epi32(1);
    int32_16 simd;

    simd.s = _mm512_or_si512(b, signBit);

    return simd;
}

inline f32_4 sqrt(int32_4 a)
{
    f32_4 simd;
    simd.s = _mm_sqrt_ps(_mm_cvtepi32_ps(a.s));

    return simd;
}

inline f32_8 sqrt(int32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_sqrt_ps(_mm256_cvtepi32_ps(a.s));

    return simd;
}

inline f32_16 sqrt(int32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_sqrt_ps(_mm512_cvtepi32_ps(a.s));

    return simd;
}

inline f32_4 sqrt_inv_approx(int32_4 a)
{
    f32_4 simd;
    simd.s = _mm_rsqrt_ps(_mm_cvtepi32_ps(a.s));

    return simd;
}

inline f32_8 sqrt_inv_approx(int32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_rsqrt_ps(_mm256_cvtepi32_ps(a.s));

    return simd;
}

inline f32_16 sqrt_inv_approx(int32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_rsqrt14_ps(_mm512_cvtepi32_ps(a.s));

    return simd;
}

inline f32_4 one_over_approx(int32_4 a)
{
    f32_4 simd;
    simd.s = _mm_rcp_ps(_mm_cvtepi32_ps(a.s));

    return simd;
}

inline f32_8 one_over_approx(int32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_rcp_ps(_mm256_cvtepi32_ps(a.s));

    return simd;
}

inline f32_16 one_over_approx(int32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_rcp14_ps(_mm512_cvtepi32_ps(a.s));

    return simd;
}

inline int32_4 clamp(int32_4 min_value, int32_4 a, int32_4 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int32_8 clamp(int32_8 min_value, int32_8 a, int32_8 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int32_16 clamp(int32_16 min_value, int32_16 a, int32_16 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int32 which_true(int32_4 a)
{
    int32 which_true = _mm_movemask_epi8(a.s);

    return which_true;
}

inline int32 which_true(int32_8 a)
{
    int32 which_true = _mm256_movemask_epi8(a.s);

    return which_true;
}

inline int32 which_true(int32_16 a)
{
    int32 which_true = _mm512_movepi32_mask(a.s);

    return which_true;
}

inline bool any_true(int32_4 a)
{
    bool is_any_true = _mm_movemask_epi8(a.s) > 0;

    return is_any_true;
}

inline bool any_true(int32_8 a)
{
    bool is_any_true = _mm256_movemask_epi8(a.s) > 0;

    return is_any_true;
}

inline bool any_true(int32_16 a)
{
    bool is_any_true = _mm512_movepi32_mask(a.s) > 0;

    return is_any_true;
}

inline bool all_true(int32_4 a)
{
    bool is_true = _mm_movemask_epi8(a.s) == 15;

    return is_true;
}

inline bool all_true(int32_8 a)
{
    bool is_true = _mm256_movemask_epi8(a.s) == 255;

    return is_true;
}

inline bool all_true(int32_16 a)
{
    bool is_true = _mm512_movepi32_mask(a.s) == 65535;

    return is_true;
}

inline bool all_false(int32_4 a)
{
    bool is_false = _mm_movemask_epi8(a.s) == 0;

    return is_false;
}

inline bool all_false(int32_8 a)
{
    bool is_false = _mm256_movemask_epi8(a.s) == 0;

    return is_false;
}

inline bool all_false(int32_16 a)
{
    // @todo This can be optimized (requires also changes in the comparison functions return)
    bool is_false = _mm512_movepi32_mask(a.s) == 0;

    return is_false;
}

// @todo from down here we can optimize some of the code by NOT using the wrappers
//      the code is self contained and we could use te intrinsic functions directly

inline
void simd_mult(const int32* a, const int32* b, int32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512i b_16;
        __m512i result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            b_16 = _mm512_loadu_epi32(b);
            result_16 = _mm512_mul_epi32(a_16, b_16);
            _mm512_store_epi32(result, result_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256i b_8;
        __m256i result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            b_8 = _mm256_loadu_epi32(b);
            result_8 = _mm256_mul_epi32(a_8, b_8);
            _mm256_store_si256((__m256i *) result, result_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128i b_4;
        __m128i result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            b_4 = _mm_loadu_epi32(b);
            result_4 = _mm_mul_epi32(a_4, b_4);
            _mm_store_si128((__m128i *) result, result_4);

            a += steps;
            b += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        ++a;
        ++b;
        ++result;

        *result = *a * *b;
    }
}

inline
void simd_mult(const int32* a, const f32* b, f32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_16;
        __m512 b_16;
        __m512 result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            af_16 = _mm512_cvtepi32_ps(a_16);
            b_16 = _mm512_loadu_ps(b);
            result_16 = _mm512_mul_ps(af_16, b_16);
            _mm512_store_ps(result, result_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256 af_8;
        __m256 b_8;
        __m256 result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            af_8 = _mm256_cvtepi32_ps(a_8);
            b_8 = _mm256_loadu_ps(b);
            result_8 = _mm256_mul_ps(af_8, b_8);
            _mm256_store_ps(result, result_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_4;
        __m128 b_4;
        __m128 result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            af_4 = _mm_cvtepi32_ps(a_4);
            b_4 = _mm_loadu_ps(b);
            result_4 = _mm_mul_ps(af_4, b_4);
            _mm_store_ps(result, result_4);

            a += steps;
            b += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = *a * *b;

        ++a;
        ++b;
        ++result;
    }
}

inline
void simd_mult(const int32* a, const f32* b, int32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_16;
        __m512 b_16;
        __m512 result_16;
        __m512i resulti_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            af_16 = _mm512_cvtepi32_ps(a_16);
            b_16 = _mm512_loadu_ps(b);
            result_16 = _mm512_mul_ps(af_16, b_16);
            resulti_16 = _mm512_cvtps_epi32(result_16);
            _mm512_store_epi32(result, resulti_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256 af_8;
        __m256 b_8;
        __m256 result_8;
        __m256i resulti_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            af_8 = _mm256_cvtepi32_ps(a_8);
            b_8 = _mm256_loadu_ps(b);
            result_8 = _mm256_mul_ps(af_8, b_8);
            resulti_8 = _mm256_cvtps_epi32(result_8);
            _mm256_store_si256((__m256i *) result, resulti_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_4;
        __m128 b_4;
        __m128 result_4;
        __m128i resulti_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            af_4 = _mm_cvtepi32_ps(a_4);
            b_4 = _mm_loadu_ps(b);
            result_4 = _mm_mul_ps(af_4, b_4);
            resulti_4 = _mm_cvtps_epi32(result_4);
            _mm_store_si128((__m128i *) result, resulti_4);

            a += steps;
            b += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = (int32) (*a * *b);

        ++a;
        ++b;
        ++result;
    }
}

inline
void simd_mult(const int32* a, f32 b, int32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_16;
        __m512 b_16 = _mm512_set1_ps(b);
        __m512 result_16;
        __m512i resulti_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            af_16 = _mm512_cvtepi32_ps(a_16);
            result_16 = _mm512_mul_ps(af_16, b_16);
            resulti_16 = _mm512_cvtps_epi32(result_16);
            _mm512_store_epi32(result, resulti_16);

            a += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256 af_8;
        __m256 b_8 = _mm256_set1_ps(b);
        __m256 result_8;
        __m256i resulti_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            af_8 = _mm256_cvtepi32_ps(a_8);
            result_8 = _mm256_mul_ps(af_8, b_8);
            resulti_8 = _mm256_cvtps_epi32(result_8);
            _mm256_store_si256((__m256i *) result, resulti_8);

            a += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_4;
        __m128 b_4 = _mm_set1_ps(b);
        __m128 result_4;
        __m128i resulti_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            af_4 = _mm_cvtepi32_ps(a_4);
            result_4 = _mm_mul_ps(af_4, b_4);
            resulti_4 = _mm_cvtps_epi32(result_4);
            _mm_store_si128((__m128i *) result, resulti_4);

            a += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = (int32) (*a * b);

        ++a;
        ++result;
    }
}

inline
void simd_div(const int32* a, f32 b, f32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_16;
        __m512 b_16 = _mm512_set1_ps(b);
        __m512 result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            af_16 = _mm512_cvtepi32_ps(a_16);
            result_16 = _mm512_div_ps(af_16, b_16);
            _mm512_store_ps(result, result_16);

            a += steps;
            result += steps;
       }
    } else if (steps == 8) {
        // @todo this his how all the functions should be implemented that take in baseic types and output basic types
        __m256i a_8;
        __m256 af_8;
        __m256 b_8 = _mm256_set1_ps(b);
        __m256 result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            af_8 = _mm256_cvtepi32_ps(a_8);
            result_8 = _mm256_div_ps(af_8, b_8);
            _mm256_store_ps(result, result_8);

            a += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_4;
        __m128 b_4 = _mm_set1_ps(b);
        __m128 result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            af_4 = _mm_cvtepi32_ps(a_4);
            result_4 = _mm_div_ps(af_4, b_4);
            _mm_store_ps(result, result_4);

            a += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = *a / b;

        ++a;
        ++result;
    }
}

inline
void simd_add(const int32* a, const int32* b, int32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512i b_16;
        __m512i result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            b_16 = _mm512_loadu_epi32(b);
            result_16 = _mm512_add_epi32(a_16, b_16);
            _mm512_store_epi32(result, result_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256i b_8;
        __m256i result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            b_8 = _mm256_loadu_epi32(b);
            result_8 = _mm256_add_epi32(a_8, b_8);
            _mm256_store_si256((__m256i *) result, result_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128i b_4;
        __m128i result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            b_4 = _mm_loadu_epi32(b);
            result_4 = _mm_add_epi32(a_4, b_4);
            _mm_store_si128((__m128i *) result, result_4);

            a += steps;
            b += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = *a + *b;

        ++a;
        ++b;
        ++result;
    }
}

inline
void simd_add(const int32* a, const f32* b, f32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_16;
        __m512 b_16;
        __m512 result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            af_16 = _mm512_cvtepi32_ps(a_16);
            b_16 = _mm512_loadu_ps(b);
            result_16 = _mm512_add_ps(af_16, b_16);
            _mm512_store_ps(result, result_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256 af_8;
        __m256 b_8;
        __m256 result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            af_8 = _mm256_cvtepi32_ps(a_8);
            b_8 = _mm256_loadu_ps(b);
            result_8 = _mm256_add_ps(af_8, b_8);
            _mm256_store_ps(result, result_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_4;
        __m128 b_4;
        __m128 result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            af_4 = _mm_cvtepi32_ps(a_4);
            b_4 = _mm_loadu_ps(b);
            result_4 = _mm_add_ps(af_4, b_4);
            _mm_store_ps(result, result_4);

            a += steps;
            b += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = *a + *b;

        ++a;
        ++b;
        ++result;
    }
}

inline
void simd_add(const int32* a, const f32* b, int32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_16;
        __m512 b_16;
        __m512 result_16;
        __m512i resulti_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_epi32(a);
            af_16 = _mm512_cvtepi32_ps(a_16);
            b_16 = _mm512_loadu_ps(b);
            result_16 = _mm512_add_ps(af_16, b_16);
            resulti_16 = _mm512_cvtps_epi32(result_16);
            _mm512_store_epi32(result, resulti_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256i a_8;
        __m256 af_8;
        __m256 b_8;
        __m256 result_8;
        __m256i resulti_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_epi32(a);
            af_8 = _mm256_cvtepi32_ps(a_8);
            b_8 = _mm256_loadu_ps(b);
            result_8 = _mm256_add_ps(af_8, b_8);
            resulti_8 = _mm256_cvtps_epi32(result_8);
            _mm256_store_si256((__m256i *) result, resulti_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_4;
        __m128 b_4;
        __m128 result_4;
        __m128i resulti_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_epi32(a);
            af_4 = _mm_cvtepi32_ps(a_4);
            b_4 = _mm_loadu_ps(b);
            result_4 = _mm_add_ps(af_4, b_4);
            resulti_4 = _mm_cvtps_epi32(result_4);
            _mm_store_si128((__m128i *) result, resulti_4);

            a += steps;
            b += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = (int32) (*a + *b);

        ++a;
        ++b;
        ++result;
    }
}

// WARNING: only works with SSE4.2
// WARNING: incl. \0 both strings must be <= 16 length
bool str_compare_avx512(const char* str1, const char* str2) {
    __m128i s1 = _mm_loadu_si128((const __m128i *)  str1);
    __m128i s2 = _mm_loadu_si128((const __m128i *)  str2);

    return _mm_cmpistrc(s1, s2, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH) == 0;
}

void
endian_swap(const int* val, int* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        const __m512i mask_512 = _mm512_setr_epi8(
            3, 2, 1, 0,  7, 6, 5, 4,    11, 10, 9, 8,  15, 14, 13, 12,
            19, 18, 17, 16,  23, 22, 21, 20, 27, 26, 25, 24,  31, 30, 29, 28,
            35, 34, 33, 32,  39, 38, 37, 36, 43, 42, 41, 40,  47, 46, 45, 44,
            51, 50, 49, 48,  55, 54, 53, 60, 59, 58, 57, 56,  64, 63, 62, 61
        );

        for (i = 0; i <= size - steps; i += steps) {
            __m512i vec = _mm512_loadu_si512((const __m512i *) (val + i));
            vec = _mm512_shuffle_epi8(vec, mask_512);

            _mm512_storeu_si512((__m512i *) (result + i), vec);
        }
    } else if (steps == 8) {
        const __m256i mask_256 = _mm256_setr_epi8(
            3, 2, 1, 0,  7, 6, 5, 4,
            11, 10, 9, 8,  15, 14, 13, 12,
            19, 18, 17, 16,  23, 22, 21, 20,
            27, 26, 25, 24,  31, 30, 29, 28
        );

        for (i = 0; i <= size - steps; i += steps) {
            __m256i vec = _mm256_loadu_si256((const __m256i *) (val + i));
            vec = _mm256_shuffle_epi8(vec, mask_256);

            _mm256_storeu_si256((__m256i *) (result + i), vec);
        }
    } else if (steps == 4) {
        const __m128i mask_128 = _mm_setr_epi8(
            3, 2, 1, 0,
            7, 6, 5, 4,
            11, 10, 9, 8,
            15, 14, 13, 12
        );

        for (i = 0; i <= size - steps; i += steps) {
             __m128i vec = _mm_loadu_si128((const __m128i *) (val + i));
            vec = _mm_shuffle_epi8(vec, mask_128);

            _mm_storeu_si128((__m128i *) (result + i), vec);
        }
    }

    for (; i < size; ++i) {
        uint32 v = ((uint32 *) val)[i];
        ((int32 *) result)[i] = ((v << 24)
            | ((v & 0xFF00) << 8)
            | ((v >> 8) & 0xFF00)
            | (v >> 24));
    }
}

#if _WIN32 || __LITTLE_ENDIAN
    #define SWAP_ENDIAN_LITTLE_SIMD(val, result, size, steps) ((void)0)
    #define SWAP_ENDIAN_BIG_SIMD(val, result, size, steps) endian_swap((val), (result), (size), (steps))
#else
    #define SWAP_ENDIAN_LITTLE_SIMD(val, result, size, steps) endian_swap((val), (result), (size), (steps))
    #define SWAP_ENDIAN_BIG_SIMD(val, result, size, steps) ((void)0)
#endif

#endif
