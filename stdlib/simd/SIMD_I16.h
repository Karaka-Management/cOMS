/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_TOS_STDLIB_SIMD_I16_H
#define TOS_TOS_STDLIB_SIMD_I16_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"

struct int16_8 {
    union {
        __m128i s;
        int16 v[8];
    };
};

struct int16_16 {
    union {
        __m256i s;
        int16 v[16];
    };
};

struct int16_32 {
    union {
        __m512i s;
        int16 v[32];
    };
};


inline int16_8 load_int16_8(const int16* mem)
{
    int16_8 simd;
    simd.s = _mm_load_si128((__m128i *) mem);

    return simd;
}

inline int16_8 init_int16_8(const int16* mem)
{
    int16_8 simd;
    simd.s = _mm_set_epi16(
        mem[0], mem[1], mem[2], mem[3],
        mem[4], mem[5], mem[6], mem[7]
    );

    return simd;
}

inline void unload_int16_8(int16_8 a, int16 *array) { _mm_store_si128((__m128i *) array, a.s); }

inline int16_16 load_int16_16(const int16* mem)
{
    int16_16 simd;
    simd.s = _mm256_load_si256((__m256i *) mem);

    return simd;
}

inline int16_16 init_int16_16(const int16* mem)
{
    int16_16 simd;
    simd.s = _mm256_set_epi16(
        mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7],
        mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15]
    );

    return simd;
}

inline void unload_int16_16(int16_16 a, int16 *array) { _mm256_store_si256((__m256i *) array, a.s); }

inline int16_32 load_int16_32(const int16* mem)
{
    int16_32 simd;
    simd.s = _mm512_load_si512((__m512i *) mem);

    return simd;
}

inline int16_32 init_int16_32(const int16* mem)
{
    int16_32 simd;
    simd.s = _mm512_set_epi16(
        mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7],
        mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15],
        mem[16], mem[17], mem[18], mem[19], mem[20], mem[21], mem[22], mem[23],
        mem[24], mem[25], mem[26], mem[27], mem[28], mem[29], mem[30], mem[31]
    );

    return simd;
}

inline void unload_int16_32(int16_32 a, int16 *array) { _mm512_storeu_epi16(array, a.s); }

inline int16_8 init_zero_int16_8()
{
    int16_8 simd;
    simd.s = _mm_setzero_si128();

    return simd;
}

inline int16_16 init_zero_int16_16()
{
    int16_16 simd;
    simd.s = _mm256_setzero_si256();

    return simd;
}

inline int16_32 init_zero_int16_32()
{
    int16_32 simd;
    simd.s = _mm512_setzero_si512();

    return simd;
}

inline int16_8 init_value_int16_8(int16 value)
{
    int16_8 simd;
    simd.s = _mm_set1_epi16(value);

    return simd;
}

inline int16_16 init_value_int16_16(int16 value)
{
    int16_16 simd;
    simd.s = _mm256_set1_epi16(value);

    return simd;
}

inline int16_32 init_value_int16_32(int16 value)
{
    int16_32 simd;
    simd.s = _mm512_set1_epi16(value);

    return simd;
}

inline int16_8 operator+(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_add_epi16(a.s, b.s);

    return simd;
}

inline int16_16 operator+(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_add_epi16(a.s, b.s);

    return simd;
}

inline int16_32 operator+(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_add_epi16(a.s, b.s);

    return simd;
}

inline int16_8 operator-(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_sub_epi16(a.s, b.s);

    return simd;
}

inline int16_8 operator-(int16_8 a) { return init_zero_int16_8() - a; }

inline int16_16 operator-(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_sub_epi16(a.s, b.s);

    return simd;
}

inline int16_16 operator-(int16_16 a) { return init_zero_int16_16() - a; }

inline int16_32 operator-(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_sub_epi16(a.s, b.s);

    return simd;
}

inline int16_32 operator-(int16_32 a) { return init_zero_int16_32() - a; }

inline int16_8 operator*(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_mul_epi32(a.s, b.s);

    return simd;
}

inline int16_16 operator*(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_mul_epi32(a.s, b.s);

    return simd;
}

inline int16_32 operator*(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mul_epi32(a.s, b.s);

    return simd;
}

inline int16_8 operator/(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_div_epi16(a.s, b.s);

    return simd;
}

inline int16_16 operator/(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_div_epi16(a.s, b.s);

    return simd;
}

inline int16_32 operator/(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_div_epi16(a.s, b.s);

    return simd;
}

inline int16_8 operator^(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_xor_si128(a.s, b.s);

    return simd;
}

inline int16_16 operator^(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_xor_si256(a.s, b.s);

    return simd;
}

inline int16_32 operator^(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_xor_si512(a.s, b.s);

    return simd;
}

inline int16_8 &operator-=(int16_8 &a, int16_8 b)
{
    a = a - b;

    return a;
}

inline int16_16 &operator-=(int16_16 &a, int16_16 b)
{
    a = a - b;

    return a;
}

inline int16_32 &operator-=(int16_32 &a, int16_32 b)
{
    a = a - b;

    return a;
}

inline int16_8 &operator+=(int16_8 &a, int16_8 b)
{
    a = a + b;

    return a;
}

inline int16_16 &operator+=(int16_16 &a, int16_16 b)
{
    a = a + b;

    return a;
}

inline int16_32 &operator+=(int16_32 &a, int16_32 b)
{
    a = a + b;

    return a;
}

inline int16_8 &operator*=(int16_8 &a, int16_8 b)
{
    a = a * b;

    return a;
}

inline int16_16 &operator*=(int16_16 &a, int16_16 b)
{
    a = a * b;

    return a;
}

inline int16_32 &operator*=(int16_32 &a, int16_32 b)
{
    a = a * b;

    return a;
}

inline int16_8 &operator/=(int16_8 &a, int16_8 b)
{
    a.s = (a / b).s;

    return a;
}

inline int16_16 &operator/=(int16_16 &a, int16_16 b)
{
    a.s = (a / b).s;

    return a;
}

inline int16_32 &operator/=(int16_32 &a, int16_32 b)
{
    a.s = (a / b).s;

    return a;
}

inline int16_8 &operator^=(int16_8 &a, int16_8 b)
{
    a = a ^ b;

    return a;
}

inline int16_16 &operator^=(int16_16 &a, int16_16 b)
{
    a = a ^ b;

    return a;
}

inline int16_32 &operator^=(int16_32 &a, int16_32 b)
{
    a = a ^ b;

    return a;
}

inline int16_8 operator<(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_cmplt_epi16(a.s, b.s);

    return simd;
}

inline int16_16 operator<(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_xor_si256(_mm256_cmpgt_epi16(a.s, b.s), _mm256_set1_epi16(-1));

    return simd;
}

inline int16_32 operator<(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mask_blend_epi16(_mm512_cmplt_epi16_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int16_8 operator<=(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_andnot_si128(_mm_cmplt_epi16(b.s, a.s), _mm_set1_epi16(-1));

    return simd;
}

inline int16_16 operator<=(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi16(a.s, b.s), _mm256_set1_epi16(-1));

    return simd;
}

inline int16_32 operator<=(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mask_blend_epi16(_mm512_knot(_mm512_cmpgt_epi16_mask(b.s, a.s)), b.s, a.s);

    return simd;
}

inline int16_8 operator>(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_cmpgt_epi16(a.s, b.s);

    return simd;
}

inline int16_16 operator>(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_cmpgt_epi16(a.s, b.s);

    return simd;
}

inline int16_32 operator>(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mask_blend_epi16(_mm512_cmpgt_epi16_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int16_8 operator>=(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_andnot_si128(_mm_cmplt_epi16(a.s, b.s), _mm_set1_epi16(-1));

    return simd;
}

inline int16_16 operator>=(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi16(b.s, a.s), _mm256_set1_epi16(-1));

    return simd;
}

inline int16_32 operator>=(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mask_blend_epi16(_mm512_cmpge_epi16_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int16_8 operator==(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_cmpeq_epi16(a.s, b.s);

    return simd;
}

inline int16_16 operator==(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_cmpeq_epi16(a.s, b.s);

    return simd;
}

inline int16_32 operator==(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mask_blend_epi16(_mm512_cmpeq_epi16_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int16_8 operator!=(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_andnot_si128(_mm_cmpeq_epi16(a.s, b.s), _mm_set1_epi16(-1));

    return simd;
}

inline int16_16 operator!=(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_mask_blend_epi16(_mm256_cmp_epi16_mask(a.s, b.s, _MM_CMPINT_NE), a.s, b.s);

    return simd;
}

inline int16_32 operator!=(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_mask_blend_epi16(_mm512_cmp_epi16_mask(a.s, b.s, _MM_CMPINT_NE), a.s, b.s);

    return simd;
}

inline int16_8 operator&(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_and_si128(a.s, b.s);

    return simd;
}

inline int16_16 operator&(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_and_si256(a.s, b.s);

    return simd;
}

inline int16_32 operator&(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_and_si512(a.s, b.s);

    return simd;
}

inline int16_8 operator|(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_or_si128(a.s, b.s);

    return simd;
}

inline int16_16 operator|(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_or_si256(a.s, b.s);

    return simd;
}

inline int16_32 operator|(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_or_si512(a.s, b.s);

    return simd;
}

inline int16_8 &operator&=(int16_8 &a, int16_8 b)
{
    a = a & b;

    return a;
}

inline int16_16 &operator&=(int16_16 &a, int16_16 b)
{
    a = a & b;

    return a;
}

inline int16_32 &operator&=(int16_32 &a, int16_32 b)
{
    a = a & b;

    return a;
}

inline int16_8 &operator|=(int16_8 &a, int16_8 b)
{
    a = a | b;

    return a;
}

inline int16_16 &operator|=(int16_16 &a, int16_16 b)
{
    a = a | b;

    return a;
}

inline int16_32 &operator|=(int16_32 &a, int16_32 b)
{
    a = a | b;

    return a;
}

inline int16_8 abs(int16_8 a)
{
    int16_8 simd;
    simd.s = _mm_abs_epi16(a.s);

    return simd;
}

inline int16_16 abs(int16_16 a)
{
    int16_16 simd;
    simd.s = _mm256_abs_epi16(a.s);

    return simd;
}

inline int16_32 abs(int16_32 a)
{
    int16_32 simd;
    simd.s = _mm512_abs_epi16(a.s);

    return simd;
}

inline int16_8 simd_min(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_min_epi16(a.s, b.s);

    return simd;
}

inline int16_16 simd_min(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_min_epi16(a.s, b.s);

    return simd;
}

inline int16_32 simd_min(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_min_epi16(a.s, b.s);

    return simd;
}

inline int16_8 simd_max(int16_8 a, int16_8 b)
{
    int16_8 simd;
    simd.s = _mm_max_epi16(a.s, b.s);

    return simd;
}

inline int16_16 simd_max(int16_16 a, int16_16 b)
{
    int16_16 simd;
    simd.s = _mm256_max_epi16(a.s, b.s);

    return simd;
}

inline int16_32 simd_max(int16_32 a, int16_32 b)
{
    int16_32 simd;
    simd.s = _mm512_max_epi16(a.s, b.s);

    return simd;
}

inline int16_8 clamp(int16_8 min_value, int16_8 a, int16_8 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int16_16 clamp(int16_16 min_value, int16_16 a, int16_16 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int16_32 clamp(int16_32 min_value, int16_32 a, int16_32 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int16 which_true(int16_8 a)
{
    int16 which_true = _mm_movemask_epi8(a.s);

    return which_true;
}

inline int16 which_true(int16_16 a)
{
    int16 which_true = _mm256_movemask_epi8(a.s);

    return which_true;
}

inline int16 which_true(int16_32 a)
{
    int16 which_true = _mm512_movepi16_mask(a.s);

    return which_true;
}

inline bool any_true(int16_8 a)
{
    bool is_any_true = _mm_movemask_epi8(a.s) > 0;

    return is_any_true;
}

inline bool any_true(int16_16 a)
{
    bool is_any_true = _mm256_movemask_epi8(a.s) > 0;

    return is_any_true;
}

inline bool any_true(int16_32 a)
{
    bool is_any_true = _mm512_movepi16_mask(a.s) > 0;

    return is_any_true;
}

inline bool all_true(int16_8 a)
{
    bool is_true = _mm_movemask_epi8(a.s) == 15;

    return is_true;
}

inline bool all_true(int16_16 a)
{
    bool is_true = _mm256_movemask_epi8(a.s) == 255;

    return is_true;
}

inline bool all_true(int16_32 a)
{
    bool is_true = _mm512_movepi16_mask(a.s) == 65535;

    return is_true;
}

inline bool all_false(int16_8 a)
{
    bool is_false = _mm_movemask_epi8(a.s) == 0;

    return is_false;
}

inline bool all_false(int16_16 a)
{
    bool is_false = _mm256_movemask_epi8(a.s) == 0;

    return is_false;
}

inline bool all_false(int16_32 a)
{
    // @todo This can be optimized (requires also changes in the comparison functions return)
    bool is_false = _mm512_movepi16_mask(a.s) == 0;

    return is_false;
}

// @todo from down here we can optimize some of the code by NOT using the wrappers
//      the code is self contained and we could use te intrinsic functions directly

inline
void simd_mult(const int16* a, f32 b, int16* result, int32 size, int32 steps)
{
    int32 i = 0;

    if (steps == 16) {
        __m512i a_16;
        __m512 af_lo, af_hi;
        __m512 b_16 = _mm512_set1_ps(b);
        __m512 result_lo, result_hi;
        __m512i result_16;

        for (; i <= size - steps; i += steps) {
            a_16 = _mm512_loadu_si512((__m512i*) a);

            af_lo = _mm512_cvtepi32_ps(_mm512_cvtepi16_epi32(_mm512_extracti64x4_epi64(a_16, 0)));
            af_hi = _mm512_cvtepi32_ps(_mm512_cvtepi16_epi32(_mm512_extracti64x4_epi64(a_16, 1)));

            result_lo = _mm512_mul_ps(af_lo, b_16);
            result_hi = _mm512_mul_ps(af_hi, b_16);

            result_16 = _mm512_packs_epi32(_mm512_cvtps_epi32(result_lo), _mm512_cvtps_epi32(result_hi));
            _mm512_storeu_si512((__m512i*) result, result_16);

            a += steps;
            result += steps;
        }
    } else if (steps == 8) {
        __m256i a_8;
        __m256 af_lo, af_hi;
        __m256 b_8 = _mm256_set1_ps(b);
        __m256 result_lo, result_hi;
        __m256i result_8;

        for (; i <= size - steps; i += steps) {
            a_8 = _mm256_loadu_si256((__m256i*) a);

            af_lo = _mm256_cvtepi32_ps(_mm256_cvtepi16_epi32(_mm256_extracti128_si256(a_8, 0)));
            af_hi = _mm256_cvtepi32_ps(_mm256_cvtepi16_epi32(_mm256_extracti128_si256(a_8, 1)));

            result_lo = _mm256_mul_ps(af_lo, b_8);
            result_hi = _mm256_mul_ps(af_hi, b_8);

            result_8 = _mm256_packs_epi32(_mm256_cvtps_epi32(result_lo), _mm256_cvtps_epi32(result_hi));
            _mm256_storeu_si256((__m256i*) result, result_8);

            a += steps;
            result += steps;
        }
    } else if (steps == 4) {
        __m128i a_4;
        __m128 af_lo, af_hi;
        __m128 b_4 = _mm_set1_ps(b);
        __m128 result_lo, result_hi;
        __m128i result_4;

        for (; i <= size - steps; i += steps) {
            a_4 = _mm_loadu_si128((__m128i*) a);

            af_lo = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(a_4));
            af_hi = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(_mm_srli_si128(a_4, 8)));

            result_lo = _mm_mul_ps(af_lo, b_4);
            result_hi = _mm_mul_ps(af_hi, b_4);

            result_4 = _mm_packs_epi32(_mm_cvtps_epi32(result_lo), _mm_cvtps_epi32(result_hi));
            _mm_storeu_si128((__m128i*) result, result_4);

            a += steps;
            result += steps;
        }
    }

    // Handle any remaining elements
    for (; i < size; ++i) {
        *result = (int16) ((f32) (*a) * b);
        ++a;
        ++result;
    }
}

#endif