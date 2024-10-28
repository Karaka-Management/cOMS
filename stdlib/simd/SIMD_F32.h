/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_SIMD_F32_H
#define TOS_STDLIB_SIMD_F32_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"
#include "SIMD_SVML.h"

struct f32_4 {
    union {
        __m128 s;
        f32 v[4];
    };
};

struct f32_8 {
    union {
        __m256 s;
        f32 v[8];
    };
};

struct f32_16 {
    union {
        __m512 s;
        f32 v[16];
    };
};

inline f32_4 load_f32_4(const f32* mem)
{
    f32_4 simd;
    simd.s = _mm_load_ps(mem);

    return simd;
}

inline f32_4 init_f32_4(const f32* mem)
{
    f32_4 simd;
    simd.s = _mm_set_ps(mem[0], mem[1], mem[2], mem[3]);

    return simd;
}

inline void unload_f32_4(f32_4 a, f32 *array) { _mm_store_ps(array, a.s); }

inline f32_8 load_f32_8(const f32* mem)
{
    f32_8 simd;
    simd.s = _mm256_load_ps(mem);

    return simd;
}

inline f32_8 init_f32_8(const f32* mem)
{
    f32_8 simd;
    simd.s = _mm256_set_ps(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]);

    return simd;
}

inline void unload_f32_8(f32_8 a, f32 *array) { _mm256_store_ps(array, a.s); }

inline f32_16 load_f32_16(const f32* mem)
{
    f32_16 simd;
    simd.s = _mm512_load_ps(mem);

    return simd;
}

inline f32_16 init_f32_16(const f32* mem)
{
    f32_16 simd;
    simd.s = _mm512_set_ps(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9], mem[10],
                            mem[11], mem[12], mem[13], mem[14], mem[15]);

    return simd;
}

inline void unload_f32_16(f32_16 a, f32 *array) { _mm512_store_ps(array, a.s); }

inline f32_4 init_zero_f32_4()
{
    f32_4 simd;
    simd.s = _mm_setzero_ps();

    return simd;
}

inline f32_8 init_zero_f32_8()
{
    f32_8 simd;
    simd.s = _mm256_setzero_ps();

    return simd;
}

inline f32_16 init_zero_f32_16()
{
    f32_16 simd;
    simd.s = _mm512_setzero_ps();

    return simd;
}

inline f32_4 init_value_f32_4(f32 value)
{
    f32_4 simd;
    simd.s = _mm_set1_ps(value);

    return simd;
}

inline f32_8 init_value_f32_8(f32 value)
{
    f32_8 simd;
    simd.s = _mm256_set1_ps(value);

    return simd;
}

inline f32_16 init_value_f32_16(f32 value)
{
    f32_16 simd;
    simd.s = _mm512_set1_ps(value);

    return simd;
}

inline f32_4 init_values_f32_4(f32 a, f32 b, f32 c, f32 d)
{
    f32_4 simd;
    simd.s = _mm_set_ps(a, b, c, d);

    return simd;
}

inline f32_8 init_values_f32_8(
    f32 a, f32 b, f32 c, f32 d,
    f32 e, f32 f, f32 g, f32 h
)
{
    f32_8 simd;
    simd.s = _mm256_set_ps(a, b, c, d, e, f, g, h);

    return simd;
}

inline f32_16 init_values_f32_16(
    f32 a, f32 b, f32 c, f32 d,
    f32 e, f32 f, f32 g, f32 h,
    f32 i, f32 j, f32 k, f32 l,
    f32 m, f32 n, f32 o, f32 p
)
{
    f32_16 simd;
    simd.s = _mm512_set_ps(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);

    return simd;
}

inline f32_4 operator+(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_add_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator+(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_add_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator+(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_add_ps(a.s, b.s);

    return simd;
}

inline f32_4 operator-(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_sub_ps(a.s, b.s);

    return simd;
}

inline f32_4 operator-(f32_4 a) { return init_zero_f32_4() - a; }

inline f32_8 operator-(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_sub_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator-(f32_8 a) { return init_zero_f32_8() - a; }

inline f32_16 operator-(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_sub_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator-(f32_16 a) { return init_zero_f32_16() - a; }

inline f32_4 operator*(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_mul_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator*(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_mul_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator*(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mul_ps(a.s, b.s);

    return simd;
}

inline f32_4 operator/(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_div_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator/(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_div_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator/(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_div_ps(a.s, b.s);

    return simd;
}

inline f32_4 operator^(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_xor_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator^(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_xor_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator^(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_xor_ps(a.s, b.s);

    return simd;
}

inline f32_4 &operator-=(f32_4 &a, f32_4 b)
{
    a = a - b;

    return a;
}

inline f32_8 &operator-=(f32_8 &a, f32_8 b)
{
    a = a - b;

    return a;
}

inline f32_16 &operator-=(f32_16 &a, f32_16 b)
{
    a = a - b;

    return a;
}

inline f32_4 &operator+=(f32_4 &a, f32_4 b)
{
    a = a + b;

    return a;
}

inline f32_8 &operator+=(f32_8 &a, f32_8 b)
{
    a = a + b;

    return a;
}

inline f32_16 &operator+=(f32_16 &a, f32_16 b)
{
    a = a + b;

    return a;
}

inline f32_4 &operator*=(f32_4 &a, f32_4 b)
{
    a = a * b;

    return a;
}

inline f32_8 &operator*=(f32_8 &a, f32_8 b)
{
    a = a * b;

    return a;
}

inline f32_16 &operator*=(f32_16 &a, f32_16 b)
{
    a = a * b;

    return a;
}

inline f32_4 &operator/=(f32_4 &a, f32_4 b)
{
    a = a / b;

    return a;
}

inline f32_8 &operator/=(f32_8 &a, f32_8 b)
{
    a = a / b;

    return a;
}

inline f32_16 &operator/=(f32_16 &a, f32_16 b)
{
    a = a / b;

    return a;
}

inline f32_4 &operator^=(f32_4 &a, f32_4 b)
{
    a = a ^ b;

    return a;
}

inline f32_8 &operator^=(f32_8 &a, f32_8 b)
{
    a = a ^ b;

    return a;
}

inline f32_16 &operator^=(f32_16 &a, f32_16 b)
{
    a = a ^ b;

    return a;
}

inline f32_4 operator<(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_cmplt_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator<(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_cmp_ps(a.s, b.s, _CMP_LT_OQ);

    return simd;
}

inline f32_16 operator<(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mask_blend_ps(_mm512_cmplt_ps_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline f32_4 operator<=(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_cmple_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator<=(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_cmp_ps(a.s, b.s, _CMP_LE_OQ);

    return simd;
}

inline f32_16 operator<=(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mask_blend_ps(_mm512_cmp_ps_mask(a.s, b.s, _CMP_LE_OQ), a.s, b.s);

    return simd;
}

inline f32_4 operator>(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_cmpgt_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator>(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_cmp_ps(a.s, b.s, _CMP_GT_OQ);

    return simd;
}

inline f32_16 operator>(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mask_blend_ps(_mm512_cmp_ps_mask(a.s, b.s, _CMP_GT_OQ), a.s, b.s);

    return simd;
}

inline f32_4 operator>=(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_cmpge_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator>=(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_cmp_ps(a.s, b.s, _CMP_GE_OQ);

    return simd;
}

inline f32_16 operator>=(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mask_blend_ps(_mm512_cmp_ps_mask(a.s, b.s, _CMP_GE_OQ), a.s, b.s);

    return simd;
}

inline f32_4 operator==(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_cmpeq_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator==(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_cmp_ps(a.s, b.s, _CMP_EQ_OQ);

    return simd;
}

inline f32_16 operator==(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mask_blend_ps(_mm512_cmp_ps_mask(a.s, b.s, _CMP_EQ_OQ), a.s, b.s);

    return simd;
}

inline f32_4 operator!=(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_cmpneq_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator!=(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_cmp_ps(a.s, b.s, _CMP_NEQ_OQ);

    return simd;
}

inline f32_16 operator!=(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_mask_blend_ps(_mm512_cmp_ps_mask(a.s, b.s, _CMP_NEQ_OQ), a.s, b.s);

    return simd;
}

inline f32_4 operator&(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_and_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator&(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_and_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator&(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_and_ps(a.s, b.s);

    return simd;
}

inline f32_4 operator|(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_or_ps(a.s, b.s);

    return simd;
}

inline f32_8 operator|(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_or_ps(a.s, b.s);

    return simd;
}

inline f32_16 operator|(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_or_ps(a.s, b.s);

    return simd;
}

inline f32_4 &operator&=(f32_4 &a, f32_4 b)
{
    a = a & b;

    return a;
}

inline f32_8 &operator&=(f32_8 &a, f32_8 b)
{
    a = a & b;

    return a;
}

inline f32_16 &operator&=(f32_16 &a, f32_16 b)
{
    a = a & b;

    return a;
}

inline f32_4 &operator|=(f32_4 &a, f32_4 b)
{
    a = a | b;

    return a;
}

inline f32_8 &operator|=(f32_8 &a, f32_8 b)
{
    a = a | b;

    return a;
}

inline f32_16 &operator|=(f32_16 &a, f32_16 b)
{
    a = a | b;

    return a;
}

inline f32_4 abs(f32_4 a)
{
    uint32 unsigned_mask = (uint32) (1U << 31);
    __m128 mask                = _mm_set1_ps(*(f32 *) &unsigned_mask);

    f32_4 simd;
    simd.s = _mm_and_ps(a.s, mask);

    return simd;
}

inline f32_8 abs(f32_8 a)
{
    uint32 unsigned_mask = (uint32) (1U << 31);
    __m256 mask                = _mm256_set1_ps(*(f32 *) &unsigned_mask);

    f32_8 simd;
    simd.s = _mm256_and_ps(a.s, mask);

    return simd;
}

inline f32_16 abs(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_abs_ps(a.s);

    return simd;
}

inline f32_4 simd_min(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_min_ps(a.s, b.s);

    return simd;
}

inline f32_8 simd_min(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_min_ps(a.s, b.s);

    return simd;
}

inline f32_16 simd_min(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_min_ps(a.s, b.s);

    return simd;
}

inline f32_4 simd_max(f32_4 a, f32_4 b)
{
    f32_4 simd;
    simd.s = _mm_max_ps(a.s, b.s);

    return simd;
}

inline f32_8 simd_max(f32_8 a, f32_8 b)
{
    f32_8 simd;
    simd.s = _mm256_max_ps(a.s, b.s);

    return simd;
}

inline f32_16 simd_max(f32_16 a, f32_16 b)
{
    f32_16 simd;
    simd.s = _mm512_max_ps(a.s, b.s);

    return simd;
}

inline f32_4 sign(f32_4 a)
{
    uint32 umask = (uint32) (1U << 31);
    __m128 mask        = _mm_set1_ps(*(f32 *) &umask);

    f32_4 signBit;
    signBit.s = _mm_and_ps(a.s, mask);

    f32_4 b;
    b.s = _mm_set1_ps(1.0f);

    f32_4 simd = b | signBit;

    return simd;
}

inline f32_8 sign(f32_8 a)
{
    uint32 umask = (uint32) (1U << 31);
    __m256 mask        = _mm256_set1_ps(*(f32 *) &umask);

    f32_8 signBit;
    signBit.s = _mm256_and_ps(a.s, mask);

    f32_8 b;
    b.s = _mm256_set1_ps(1.0f);

    f32_8 simd = b | signBit;

    return simd;
}

inline f32_16 sign(f32_16 a)
{
    uint32 umask = (uint32) (1U << 31);
    __m512 mask        = _mm512_set1_ps(*(f32 *) &umask);

    f32_16 signBit;
    signBit.s = _mm512_and_ps(a.s, mask);

    f32_16 b;
    b.s = _mm512_set1_ps(1.0f);

    f32_16 simd = b | signBit;

    return simd;
}

inline f32_4 floor(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_floor_ps(a.s);

    return simd;
}

inline f32_8 floor(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_floor_ps(a.s);

    return simd;
}

inline f32_16 floor(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_floor_ps(a.s);

    return simd;
}

inline f32_4 ceil(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_ceil_ps(a.s);

    return simd;
}

inline f32_8 ceil(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_ceil_ps(a.s);

    return simd;
}

inline f32_16 ceil(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_ceil_ps(a.s);

    return simd;
}

inline f32_4 sqrt(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_sqrt_ps(a.s);

    return simd;
}

inline f32_8 sqrt(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_sqrt_ps(a.s);

    return simd;
}

inline f32_16 sqrt(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_sqrt_ps(a.s);

    return simd;
}

inline f32_4 sqrt_inv_approx(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_rsqrt_ps(a.s);

    return simd;
}

inline f32_8 sqrt_inv_approx(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_rsqrt_ps(a.s);

    return simd;
}

inline f32_16 sqrt_inv_approx(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_rsqrt14_ps(a.s);

    return simd;
}

inline f32_4 one_over_approx(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_rcp_ps(a.s);

    return simd;
}

inline f32_8 one_over_approx(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_rcp_ps(a.s);

    return simd;
}

inline f32_16 one_over_approx(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_rcp14_ps(a.s);

    return simd;
}

inline f32_4 clamp(f32_4 min_value, f32_4 a, f32_4 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline f32_8 clamp(f32_8 min_value, f32_8 a, f32_8 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline f32_16 clamp(f32_16 min_value, f32_16 a, f32_16 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int32 which_true(f32_4 a)
{
    int32 which_true = _mm_movemask_ps(a.s);

    return which_true;
}

inline int32 which_true(f32_8 a)
{
    int32 which_true = _mm256_movemask_ps(a.s);

    return which_true;
}

inline int32 which_true(f32_16 a)
{
    int32 which_true = _mm512_movepi32_mask(_mm512_castps_si512(a.s));

    return which_true;
}

inline bool any_true(f32_4 a)
{
    bool is_any_true = _mm_movemask_ps(a.s) > 0;

    return is_any_true;
}

inline bool any_true(f32_8 a)
{
    bool is_any_true = _mm256_movemask_ps(a.s) > 0;

    return is_any_true;
}

inline bool any_true(f32_16 a)
{
    bool is_any_true = _mm512_movepi32_mask(_mm512_castps_si512(a.s)) > 0;

    return is_any_true;
}

inline bool all_true(f32_4 a)
{
    bool is_true = _mm_movemask_ps(a.s) == 15;

    return is_true;
}

inline bool all_true(f32_8 a)
{
    bool is_true = _mm256_movemask_ps(a.s) == 255;

    return is_true;
}

inline bool all_true(f32_16 a)
{
    bool is_true = _mm512_movepi32_mask(_mm512_castps_si512(a.s)) == 65535;

    return is_true;
}

inline bool all_false(f32_4 a)
{
    bool is_false = _mm_movemask_ps(a.s) == 0;

    return is_false;
}

inline bool all_false(f32_8 a)
{
    bool is_false = _mm256_movemask_ps(a.s) == 0;

    return is_false;
}

inline bool all_false(f32_16 a)
{
    // @todo This can be optimized (requires also changes in the comparison functions return)
    bool is_false = _mm512_movepi32_mask(_mm512_castps_si512(a.s)) == 0;

    return is_false;
}

// @todo from down here we can optimize some of the code by NOT using the wrappers
//      the code is self contained and we could use te intrinsic functions directly

inline
void simd_mult(const f32* a, const f32* b, f32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512 a_16;
        __m512 b_16;
        __m512 result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_load_ps(a);
            b_16 = _mm512_load_ps(b);
            result_16 = _mm512_mul_ps(a_16, b_16);
            _mm512_store_ps(result, result_16);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256 a_8;
        __m256 b_8;
        __m256 result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_load_ps(a);
            b_8 = _mm256_load_ps(b);
            result_8 = _mm256_mul_ps(a_8, b_8);
            _mm256_store_ps(result, result_8);

            a += steps;
            b += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128 a_4;
        __m128 b_4;
        __m128 result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_load_ps(a);
            b_4 = _mm_load_ps(b);
            result_4 = _mm_mul_ps(a_4, b_4);
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
void simd_mult(const f32* a, f32 b, f32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512 a_16;
        __m512 b_16 = _mm512_set1_ps(b);
        __m512 result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_load_ps(a);
            result_16 = _mm512_mul_ps(a_16, b_16);
            _mm512_store_ps(result, result_16);

            a += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256 a_8;
        __m256 b_8 = _mm256_set1_ps(b);
        __m256 result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_load_ps(a);
            result_8 = _mm256_mul_ps(a_8, b_8);
            _mm256_store_ps(result, result_8);

            a += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128 a_4;
        __m128 b_4 = _mm_set1_ps(b);
        __m128 result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_load_ps(a);
            result_4 = _mm_mul_ps(a_4, b_4);
            _mm_store_ps(result, result_4);

            a += steps;
            result += steps;
       }
    }

    for (; i < size; ++i) {
        *result = *a * b;

        ++a;
        ++result;
    }
}

inline
void simd_div(const f32* a, f32 b, f32* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512 a_16;
        __m512 b_16 = _mm512_set1_ps(b);
        __m512 result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_load_ps(a);
            result_16 = _mm512_div_ps(a_16, b_16);
            _mm512_store_ps(result, result_16);

            a += steps;
            result += steps;
       }
    } else if (steps == 8) {
        __m256 a_8;
        __m256 b_8 = _mm256_set1_ps(b);
        __m256 result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_load_ps(a);
            result_8 = _mm256_div_ps(a_8, b_8);
            _mm256_store_ps(result, result_8);

            a += steps;
            result += steps;
       }
    } else if (steps == 4) {
        __m128 a_4;
        __m128 b_4 = _mm_set1_ps(b);
        __m128 result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_load_ps(a);
            result_4 = _mm_div_ps(a_4, b_4);
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
void simd_div(const f32* a, f32 b, __m256* result, int32 size)
{
    int32 i = 0;
    int32 j = 0;

    // @todo this his how all the functions should be implemented that take in baseic types and output basic types
    __m256 a_8;
    __m256 b_8 = _mm256_set1_ps(b);
    __m256 result_8;

    for (; i <= size - 8; i += 8) {
        a_8 = _mm256_load_ps(a);
        result_8 = _mm256_div_ps(a_8, b_8);
        result[j] = result_8;

        a += 8;
        ++j;
    }

    int32 diff = size - i;
    alignas(32) f32 temp[8];

    for (int32 k = 0; k < diff; k++) {
        temp[k] = a[i + k] / b;
    }

    result[j] = _mm256_load_ps(temp);
}

inline
void simd_cmp_le(const __m256* a, f32 b, bool* result, int32 size)
{
    __m256 b_8 = _mm256_set1_ps(b);

    for (int32 i = 0; i < size; ++i) {
        int32 mask = _mm256_movemask_ps(_mm256_cmp_ps(a[i], b_8, _CMP_LE_OQ));

        for (int32 j = 0; j < 8; ++j) {
            result[i * 8 + j] = (mask & (1 << j)) != 0;
        }
    }
}

// @todo But a guard or warning on the trigonometric functions since they are only implemented for msvc/intel compiler
inline
f32_4 simd_sin(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_sin_ps(a.s);

    return simd;
}

inline
f32_8 simd_sin(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_sin_ps(a.s);

    return simd;
}

inline
f32_16 simd_sin(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_sin_ps(a.s);

    return simd;
}

inline
f32_4 simd_cos(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_cos_ps(a.s);

    return simd;
}

inline
f32_8 simd_cos(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_cos_ps(a.s);

    return simd;
}

inline
f32_16 simd_cos(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_cos_ps(a.s);

    return simd;
}

inline
f32_4 simd_asin(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_asin_ps(a.s);

    return simd;
}

inline
f32_8 simd_asin(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_asin_ps(a.s);

    return simd;
}

inline
f32_16 simd_asin(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_asin_ps(a.s);

    return simd;
}

inline
f32_4 simd_acos(f32_4 a)
{
    f32_4 simd;
    simd.s = _mm_acos_ps(a.s);

    return simd;
}

inline
f32_8 simd_acos(f32_8 a)
{
    f32_8 simd;
    simd.s = _mm256_acos_ps(a.s);

    return simd;
}

inline
f32_16 simd_acos(f32_16 a)
{
    f32_16 simd;
    simd.s = _mm512_acos_ps(a.s);

    return simd;
}

// @todo implement more trigonometry function

#endif
