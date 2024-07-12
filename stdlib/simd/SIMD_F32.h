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

inline f32_4 load_f32_4(f32 *mem)
{
    f32_4 simd;
    simd.s = _mm_loadu_ps(mem);

    return simd;
}

inline f32_4 init_f32_4(f32 *mem)
{
    f32_4 simd;
    simd.s = _mm_set_ps(mem[0], mem[1], mem[2], mem[3]);

    return simd;
}

inline void unload_f32_4(f32_4 a, f32 *array) { _mm_store_ps(array, a.s); }

inline f32_8 load_f32_8(f32 *mem)
{
    f32_8 simd;
    simd.s = _mm256_loadu_ps(mem);

    return simd;
}

inline f32_8 init_f32_8(f32 *mem)
{
    f32_8 simd;
    simd.s = _mm256_set_ps(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]);

    return simd;
}

inline void unload_f32_8(f32_8 a, f32 *array) { _mm256_store_ps(array, a.s); }

inline f32_16 load_f32_16(f32 *mem)
{
    f32_16 simd;
    simd.s = _mm512_loadu_ps(mem);

    return simd;
}

inline f32_16 init_f32_16(f32 *mem)
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
    unsigned int unsigned_mask = (unsigned int) (1 << 31);
    __m128 mask                = _mm_set1_ps(*(float *) &unsigned_mask);

    f32_4 simd;
    simd.s = _mm_and_ps(a.s, mask);

    return simd;
}

inline f32_8 abs(f32_8 a)
{
    unsigned int unsigned_mask = (unsigned int) (1 << 31);
    __m256 mask                = _mm256_set1_ps(*(float *) &unsigned_mask);

    f32_8 simd;
    simd.s = _mm256_and_ps(a.s, mask);

    return simd;
}

inline f32_16 abs(f32_16 a)
{
    unsigned int unsigned_mask = (unsigned int) (1 << 31);
    __m512 mask                = _mm512_set1_ps(*(float *) &unsigned_mask);

    f32_16 simd;
    simd.s = _mm512_and_ps(a.s, mask);

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
    unsigned int umask = (unsigned int) (1 << 31);
    __m128 mask        = _mm_set1_ps(*(float *) &umask);

    f32_4 signBit;
    signBit.s = _mm_and_ps(a.s, mask);

    f32_4 b;
    b.s = _mm_set1_ps(1.0f);

    f32_4 simd = b | signBit;

    return simd;
}

inline f32_8 sign(f32_8 a)
{
    unsigned int umask = (unsigned int) (1 << 31);
    __m256 mask        = _mm256_set1_ps(*(float *) &umask);

    f32_8 signBit;
    signBit.s = _mm256_and_ps(a.s, mask);

    f32_8 b;
    b.s = _mm256_set1_ps(1.0f);

    f32_8 simd = b | signBit;

    return simd;
}

inline f32_16 sign(f32_16 a)
{
    unsigned int umask = (unsigned int) (1 << 31);
    __m512 mask        = _mm512_set1_ps(*(float *) &umask);

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

#endif
