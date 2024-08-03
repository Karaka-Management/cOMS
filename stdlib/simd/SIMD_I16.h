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
    simd.s = _mm_loadu_epi16(mem);

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
    simd.s = _mm256_loadu_epi16(mem);

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
    simd.s = _mm512_loadu_epi16(mem);

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
void simd_mult(const int16* a, const int16* b, int16* result, int size, int steps)
{
    int i = 0;

    if (steps == 16) {
        int16_32 a_16;
        int16_32 b_16;
        int16_32 result_16;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_16 = load_int16_32(a);
            b_16 = load_int16_32(b);
            result_16 = a_16 * b_16;
            unload_int16_32(result_16, result);
       }
    } else if (steps == 8) {
        int16_16 a_8;
        int16_16 b_8;
        int16_16 result_8;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_8 = load_int16_16(a);
            b_8 = load_int16_16(b);
            result_8 = a_8 * b_8;
            unload_int16_16(result_8, result);
       }
    } else if (steps == 4) {
        int16_8 a_4;
        int16_8 b_4;
        int16_8 result_4;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_4 = load_int16_8(a);
            b_4 = load_int16_8(b);
            result_4 = a_4 * b_4;
            unload_int16_8(result_4, result);
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
void simd_mult(const int16* a, const int16* b, int16* result)
{
    int16_8 a_4 = load_int16_8(a);
    int16_8 b_4 = load_int16_8(b);
    int16_8 result_4 = a_4 * b_4;

    unload_int16_8(result_4, result);
}

inline
void int16_16_mult(const int16* a, const int16* b, int16* result)
{
    int16_16 a_8 = load_int16_16(a);
    int16_16 b_8 = load_int16_16(b);
    int16_16 result_8 = a_8 * b_8;

    unload_int16_16(result_8, result);
}

inline
void int16_32_mult(const int16* a, const int16* b, int16* result)
{
    int16_32 a_16 = load_int16_32(a);
    int16_32 b_16 = load_int16_32(b);
    int16_32 result_16 = a_16 * b_16;

    unload_int16_32(result_16, result);
}

inline
void simd_add(const int16* a, const int16* b, int16* result, int size, int steps)
{
    int i = 0;

    if (steps == 16) {
        int16_32 a_16;
        int16_32 b_16;
        int16_32 result_16;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_16 = load_int16_32(a);
            b_16 = load_int16_32(b);
            result_16 = a_16 + b_16;
            unload_int16_32(result_16, result);
       }
    } else if (steps == 8) {
        int16_16 a_8;
        int16_16 b_8;
        int16_16 result_8;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_8 = load_int16_16(a);
            b_8 = load_int16_16(b);
            result_8 = a_8 + b_8;
            unload_int16_16(result_8, result);
       }
    } else if (steps == 4) {
        int16_8 a_4;
        int16_8 b_4;
        int16_8 result_4;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_4 = load_int16_8(a);
            b_4 = load_int16_8(b);
            result_4 = a_4 + b_4;
            unload_int16_8(result_4, result);
       }
    }

    for (; i < size; ++i) {
        ++a;
        ++b;
        ++result;

        *result = *a + *b;
    }
}

inline
void int16_8_add(const int16* a, const int16* b, int16* result)
{
    int16_8 a_4 = load_int16_8(a);
    int16_8 b_4 = load_int16_8(b);
    int16_8 result_4 = a_4 + b_4;

    unload_int16_8(result_4, result);
}

inline
void int16_16_add(const int16* a, const int16* b, int16* result)
{
    int16_16 a_8 = load_int16_16(a);
    int16_16 b_8 = load_int16_16(b);
    int16_16 result_8 = a_8 + b_8;

    unload_int16_16(result_8, result);
}

inline
void int16_32_add(const int16* a, const int16* b, int16* result)
{
    int16_32 a_16 = load_int16_32(a);
    int16_32 b_16 = load_int16_32(b);
    int16_32 result_16 = a_16 + b_16;

    unload_int16_32(result_16, result);
}

// @todo add more operations like the one above "int16_8_mult()"

#endif