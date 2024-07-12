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
#include "SIMD_F32.h"

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

inline int32_4 load_int32_4(int32 *mem)
{
    int32_4 simd;
    simd.s = _mm_loadu_epi32(mem);

    return simd;
}

inline int32_4 init_int32_4(int32 *mem)
{
    int32_4 simd;
    simd.s = _mm_set_epi32(mem[0], mem[1], mem[2], mem[3]);

    return simd;
}

inline void unload_int32_4(int32_4 a, int32 *array) { _mm_store_si128((__m128i *) array, a.s); }

inline int32_8 load_int32_8(int32 *mem)
{
    int32_8 simd;
    simd.s = _mm256_loadu_epi32(mem);

    return simd;
}

inline int32_8 init_int32_8(int32 *mem)
{
    int32_8 simd;
    simd.s = _mm256_set_epi32(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]);

    return simd;
}

inline void unload_int32_8(int32_8 a, int32 *array) { _mm256_store_si256((__m256i *) array, a.s); }

inline int32_16 load_int32_16(int32 *mem)
{
    int32_16 simd;
    simd.s = _mm512_loadu_epi32(mem);

    return simd;
}

inline int32_16 init_int32_16(int32 *mem)
{
    int32_16 simd;
    simd.s = _mm512_set_epi32(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9],
                                mem[10], mem[11], mem[12], mem[13], mem[14], mem[15]);

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

inline f32_4 operator/(int32_4 a, int32_4 b)
{
    f32_4 simd;
    simd.s = _mm_div_ps(_mm_cvtepi32_ps(a.s), _mm_cvtepi32_ps(b.s));

    return simd;
}

inline f32_8 operator/(int32_8 a, int32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_div_ps(_mm256_cvtepi32_ps(a.s), _mm256_cvtepi32_ps(b.s));

    return simd;
}

inline f32_16 operator/(int32_16 a, int32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_div_ps(_mm512_cvtepi32_ps(a.s), _mm512_cvtepi32_ps(b.s));

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
    a.s = _mm_cvtps_epi32((a / b).s);

    return a;
}

inline int32_8 &operator/=(int32_8 &a, int32_8 b)
{
    a.s = _mm256_cvtps_epi32((a / b).s);

    return a;
}

inline int32_16 &operator/=(int32_16 &a, int32_16 b)
{
    a.s = _mm512_cvtps_epi32((a / b).s);

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
    simd.s = _mm_and_epi32(a.s, b.s);

    return simd;
}

inline int32_8 operator&(int32_8 a, int32_8 b)
{
    int32_8 simd;
    simd.s = _mm256_and_epi32(a.s, b.s);

    return simd;
}

inline int32_16 operator&(int32_16 a, int32_16 b)
{
    int32_16 simd;
    simd.s = _mm512_and_epi32(a.s, b.s);

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
    __m128i mask = _mm_set1_epi32(0x7FFFFFFF);

    int32_4 simd;
    simd.s = _mm_and_si128(a.s, mask);

    return simd;
}

inline int32_8 abs(int32_8 a)
{
    __m256i mask = _mm256_set1_epi32(0x7FFFFFFF);
    int32_8 simd;
    simd.s = _mm256_and_si256(a.s, mask);

    return simd;
}

inline int32_16 abs(int32_16 a)
{
    __m512i mask = _mm512_set1_epi32(0x7FFFFFFF);
    int32_16 simd;
    simd.s = _mm512_and_epi32(a.s, mask);

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
    __m128i signBit = _mm_and_epi32(a.s, mask);
    __m128i b = _mm_set1_epi32(1);

    int32_4 simd;
    simd.s = _mm_or_si128(b, signBit);

    return simd;
}

inline int32_8 sign(int32_8 a)
{
    __m256i mask = _mm256_set1_epi32(0x80000000);
    __m256i signBit = _mm256_and_epi32(a.s, mask);
    __m256i b = _mm256_set1_epi32(1);

    int32_8 simd;
    simd.s = _mm256_or_si256(b, signBit);

    return simd;
}

inline int32_16 sign(int32_16 a)
{
    __m512i mask = _mm512_set1_epi32(0x80000000);
    __m512i signBit = _mm512_and_epi32(a.s, mask);
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

#endif
