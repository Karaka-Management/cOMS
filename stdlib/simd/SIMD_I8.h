/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_TOS_STDLIB_SIMD_I8_H
#define TOS_TOS_STDLIB_SIMD_I8_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"

struct int8_16 {
    union {
        __m128i s;
        int8 v[16];
    };
};

struct int8_32 {
    union {
        __m256i s;
        int8 v[32];
    };
};

struct int8_64 {
    union {
        __m512i s;
        int8 v[64];
    };
};

inline int8_16 load_int8_16(const int8* mem)
{
    int8_16 simd;
    simd.s = _mm_loadu_epi8(mem);

    return simd;
}

inline int8_16 init_int8_16(const int8* mem)
{
    int8_16 simd;
    simd.s = _mm_set_epi8(
        mem[0], mem[1], mem[2], mem[3],
        mem[4], mem[5], mem[6], mem[7],
        mem[8], mem[9], mem[10], mem[11],
        mem[12], mem[13], mem[14], mem[15]
    );

    return simd;
}

inline void unload_int8_16(int8_16 a, int8 *array) { _mm_store_si128((__m128i *) array, a.s); }

inline int8_32 load_int8_32(const int8* mem)
{
    int8_32 simd;
    simd.s = _mm256_loadu_epi8(mem);

    return simd;
}

inline int8_32 init_int8_32(const int8* mem)
{
    int8_32 simd;
    simd.s = _mm256_set_epi8(
        mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7],
        mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15],
        mem[16], mem[17], mem[18], mem[19], mem[20], mem[21], mem[22], mem[23],
        mem[24], mem[25], mem[26], mem[27], mem[28], mem[29], mem[30], mem[31]
    );

    return simd;
}

inline void unload_int8_32(int8_32 a, int8 *array) { _mm256_store_si256((__m256i *) array, a.s); }

inline int8_64 load_int8_64(const int8* mem)
{
    int8_64 simd;
    simd.s = _mm512_loadu_epi8(mem);

    return simd;
}

inline int8_64 init_int8_64(const int8* mem)
{
    int8_64 simd;
    simd.s = _mm512_set_epi8(
        mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7],
        mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15],
        mem[16], mem[17], mem[18], mem[19], mem[20], mem[21], mem[22], mem[23],
        mem[24], mem[25], mem[26], mem[27], mem[28], mem[29], mem[30], mem[31],
        mem[32], mem[33], mem[34], mem[35], mem[36], mem[37], mem[38], mem[39],
        mem[40], mem[41], mem[42], mem[43], mem[44], mem[45], mem[46], mem[47],
        mem[48], mem[49], mem[50], mem[51], mem[52], mem[53], mem[54], mem[55],
        mem[56], mem[57], mem[58], mem[59], mem[60], mem[61], mem[62], mem[63]
    );

    return simd;
}

inline void unload_int8_64(int8_64 a, int8 *array) { _mm512_storeu_epi8(array, a.s); }

inline int8_16 init_zero_int8_16()
{
    int8_16 simd;
    simd.s = _mm_setzero_si128();

    return simd;
}

inline int8_32 init_zero_int8_32()
{
    int8_32 simd;
    simd.s = _mm256_setzero_si256();

    return simd;
}

inline int8_64 init_zero_int8_64()
{
    int8_64 simd;
    simd.s = _mm512_setzero_si512();

    return simd;
}

inline int8_16 init_value_int8_16(int8 value)
{
    int8_16 simd;
    simd.s = _mm_set1_epi8(value);

    return simd;
}

inline int8_32 init_value_int8_32(int8 value)
{
    int8_32 simd;
    simd.s = _mm256_set1_epi8(value);

    return simd;
}

inline int8_64 init_value_int8_64(int8 value)
{
    int8_64 simd;
    simd.s = _mm512_set1_epi8(value);

    return simd;
}

inline int8_16 operator+(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_add_epi8(a.s, b.s);

    return simd;
}

inline int8_32 operator+(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_add_epi8(a.s, b.s);

    return simd;
}

inline int8_64 operator+(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_add_epi8(a.s, b.s);

    return simd;
}

inline int8_16 operator-(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_sub_epi8(a.s, b.s);

    return simd;
}

inline int8_16 operator-(int8_16 a) { return init_zero_int8_16() - a; }

inline int8_32 operator-(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_sub_epi8(a.s, b.s);

    return simd;
}

inline int8_32 operator-(int8_32 a) { return init_zero_int8_32() - a; }

inline int8_64 operator-(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_sub_epi8(a.s, b.s);

    return simd;
}

inline int8_64 operator-(int8_64 a) { return init_zero_int8_64() - a; }

inline int8_16 operator*(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_mul_epi32(a.s, b.s);

    return simd;
}

inline int8_32 operator*(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_mul_epi32(a.s, b.s);

    return simd;
}

inline int8_64 operator*(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mul_epi32(a.s, b.s);

    return simd;
}

inline int8_16 operator/(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_div_epi8(a.s, b.s);

    return simd;
}

inline int8_32 operator/(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_div_epi8(a.s, b.s);

    return simd;
}

inline int8_64 operator/(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_div_epi8(a.s, b.s);

    return simd;
}

inline int8_16 operator^(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_xor_si128(a.s, b.s);

    return simd;
}

inline int8_32 operator^(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_xor_si256(a.s, b.s);

    return simd;
}

inline int8_64 operator^(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_xor_si512(a.s, b.s);

    return simd;
}

inline int8_16 &operator-=(int8_16 &a, int8_16 b)
{
    a = a - b;

    return a;
}

inline int8_32 &operator-=(int8_32 &a, int8_32 b)
{
    a = a - b;

    return a;
}

inline int8_64 &operator-=(int8_64 &a, int8_64 b)
{
    a = a - b;

    return a;
}

inline int8_16 &operator+=(int8_16 &a, int8_16 b)
{
    a = a + b;

    return a;
}

inline int8_32 &operator+=(int8_32 &a, int8_32 b)
{
    a = a + b;

    return a;
}

inline int8_64 &operator+=(int8_64 &a, int8_64 b)
{
    a = a + b;

    return a;
}

inline int8_16 &operator*=(int8_16 &a, int8_16 b)
{
    a = a * b;

    return a;
}

inline int8_32 &operator*=(int8_32 &a, int8_32 b)
{
    a = a * b;

    return a;
}

inline int8_64 &operator*=(int8_64 &a, int8_64 b)
{
    a = a * b;

    return a;
}

inline int8_16 &operator/=(int8_16 &a, int8_16 b)
{
    a.s = (a / b).s;

    return a;
}

inline int8_32 &operator/=(int8_32 &a, int8_32 b)
{
    a.s = (a / b).s;

    return a;
}

inline int8_64 &operator/=(int8_64 &a, int8_64 b)
{
    a.s = (a / b).s;

    return a;
}

inline int8_16 &operator^=(int8_16 &a, int8_16 b)
{
    a = a ^ b;

    return a;
}

inline int8_32 &operator^=(int8_32 &a, int8_32 b)
{
    a = a ^ b;

    return a;
}

inline int8_64 &operator^=(int8_64 &a, int8_64 b)
{
    a = a ^ b;

    return a;
}

inline int8_16 operator<(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_cmplt_epi8(a.s, b.s);

    return simd;
}

inline int8_32 operator<(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_xor_si256(_mm256_cmpgt_epi8(a.s, b.s), _mm256_set1_epi8(-1));

    return simd;
}

inline int8_64 operator<(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mask_blend_epi8(_mm512_cmplt_epi8_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int8_16 operator<=(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_andnot_si128(_mm_cmplt_epi8(b.s, a.s), _mm_set1_epi8(-1));

    return simd;
}

inline int8_32 operator<=(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi8(a.s, b.s), _mm256_set1_epi8(-1));

    return simd;
}

inline int8_64 operator<=(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mask_blend_epi8(_mm512_knot(_mm512_cmpgt_epi8_mask(b.s, a.s)), b.s, a.s);

    return simd;
}

inline int8_16 operator>(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_cmpgt_epi8(a.s, b.s);

    return simd;
}

inline int8_32 operator>(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_cmpgt_epi8(a.s, b.s);

    return simd;
}

inline int8_64 operator>(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mask_blend_epi8(_mm512_cmpgt_epi8_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int8_16 operator>=(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_andnot_si128(_mm_cmplt_epi8(a.s, b.s), _mm_set1_epi8(-1));

    return simd;
}

inline int8_32 operator>=(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi8(b.s, a.s), _mm256_set1_epi8(-1));

    return simd;
}

inline int8_64 operator>=(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mask_blend_epi8(_mm512_cmpge_epi8_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int8_16 operator==(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_cmpeq_epi8(a.s, b.s);

    return simd;
}

inline int8_32 operator==(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_cmpeq_epi8(a.s, b.s);

    return simd;
}

inline int8_64 operator==(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mask_blend_epi8(_mm512_cmpeq_epi8_mask(a.s, b.s), a.s, b.s);

    return simd;
}

inline int8_16 operator!=(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_andnot_si128(_mm_cmpeq_epi8(a.s, b.s), _mm_set1_epi8(-1));

    return simd;
}

inline int8_32 operator!=(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_mask_blend_epi8(_mm256_cmp_epi8_mask(a.s, b.s, _MM_CMPINT_NE), a.s, b.s);

    return simd;
}

inline int8_64 operator!=(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_mask_blend_epi8(_mm512_cmp_epi8_mask(a.s, b.s, _MM_CMPINT_NE), a.s, b.s);

    return simd;
}

inline int8_16 operator&(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_and_si128(a.s, b.s);

    return simd;
}

inline int8_32 operator&(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_and_si256(a.s, b.s);

    return simd;
}

inline int8_64 operator&(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_and_si512(a.s, b.s);

    return simd;
}

inline int8_16 operator|(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_or_si128(a.s, b.s);

    return simd;
}

inline int8_32 operator|(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_or_si256(a.s, b.s);

    return simd;
}

inline int8_64 operator|(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_or_si512(a.s, b.s);

    return simd;
}

inline int8_16 &operator&=(int8_16 &a, int8_16 b)
{
    a = a & b;

    return a;
}

inline int8_32 &operator&=(int8_32 &a, int8_32 b)
{
    a = a & b;

    return a;
}

inline int8_64 &operator&=(int8_64 &a, int8_64 b)
{
    a = a & b;

    return a;
}

inline int8_16 &operator|=(int8_16 &a, int8_16 b)
{
    a = a | b;

    return a;
}

inline int8_32 &operator|=(int8_32 &a, int8_32 b)
{
    a = a | b;

    return a;
}

inline int8_64 &operator|=(int8_64 &a, int8_64 b)
{
    a = a | b;

    return a;
}

inline int8_16 abs(int8_16 a)
{
    int8_16 simd;
    simd.s = _mm_abs_epi8(a.s);

    return simd;
}

inline int8_32 abs(int8_32 a)
{
    int8_32 simd;
    simd.s = _mm256_abs_epi16(a.s);

    return simd;
}

inline int8_64 abs(int8_64 a)
{
    int8_64 simd;
    simd.s = _mm512_abs_epi16(a.s);

    return simd;
}

inline int8_16 simd_min(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_min_epi8(a.s, b.s);

    return simd;
}

inline int8_32 simd_min(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_min_epi8(a.s, b.s);

    return simd;
}

inline int8_64 simd_min(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_min_epi8(a.s, b.s);

    return simd;
}

inline int8_16 simd_max(int8_16 a, int8_16 b)
{
    int8_16 simd;
    simd.s = _mm_max_epi8(a.s, b.s);

    return simd;
}

inline int8_32 simd_max(int8_32 a, int8_32 b)
{
    int8_32 simd;
    simd.s = _mm256_max_epi8(a.s, b.s);

    return simd;
}

inline int8_64 simd_max(int8_64 a, int8_64 b)
{
    int8_64 simd;
    simd.s = _mm512_max_epi8(a.s, b.s);

    return simd;
}

inline int8_16 clamp(int8_16 min_value, int8_16 a, int8_16 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int8_32 clamp(int8_32 min_value, int8_32 a, int8_32 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int8_64 clamp(int8_64 min_value, int8_64 a, int8_64 max_value)
{
    return simd_min(simd_max(a, min_value), max_value);
}

inline int8 which_true(int8_16 a)
{
    int8 which_true = _mm_movemask_epi8(a.s);

    return which_true;
}

inline int8 which_true(int8_32 a)
{
    int8 which_true = _mm256_movemask_epi8(a.s);

    return which_true;
}

inline int8 which_true(int8_64 a)
{
    int8 which_true = _mm512_movepi8_mask(a.s);

    return which_true;
}

inline bool any_true(int8_16 a)
{
    bool is_any_true = _mm_movemask_epi8(a.s) > 0;

    return is_any_true;
}

inline bool any_true(int8_32 a)
{
    bool is_any_true = _mm256_movemask_epi8(a.s) > 0;

    return is_any_true;
}

inline bool any_true(int8_64 a)
{
    bool is_any_true = _mm512_movepi8_mask(a.s) > 0;

    return is_any_true;
}

inline bool all_true(int8_16 a)
{
    bool is_true = _mm_movemask_epi8(a.s) == 15;

    return is_true;
}

inline bool all_true(int8_32 a)
{
    bool is_true = _mm256_movemask_epi8(a.s) == 255;

    return is_true;
}

inline bool all_true(int8_64 a)
{
    bool is_true = _mm512_movepi8_mask(a.s) == 65535;

    return is_true;
}

inline bool all_false(int8_16 a)
{
    bool is_false = _mm_movemask_epi8(a.s) == 0;

    return is_false;
}

inline bool all_false(int8_32 a)
{
    bool is_false = _mm256_movemask_epi8(a.s) == 0;

    return is_false;
}

inline bool all_false(int8_64 a)
{
    // @todo This can be optimized (requires also changes in the comparison functions return)
    bool is_false = _mm512_movepi8_mask(a.s) == 0;

    return is_false;
}

// @todo from down here we can optimize some of the code by NOT using the wrappers
//      the code is self contained and we could use te intrinsic functions directly

inline
void simd_mult(const int8* a, const int8* b, int8* result, int size, int steps)
{
    int i = 0;

    if (steps == 16) {
        int8_64 a_16;
        int8_64 b_16;
        int8_64 result_16;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_16 = load_int8_64(a);
            b_16 = load_int8_64(b);
            result_16 = a_16 * b_16;
            unload_int8_64(result_16, result);
       }
    } else if (steps == 8) {
        int8_32 a_8;
        int8_32 b_8;
        int8_32 result_8;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_8 = load_int8_32(a);
            b_8 = load_int8_32(b);
            result_8 = a_8 * b_8;
            unload_int8_32(result_8, result);
       }
    } else if (steps == 4) {
        int8_16 a_4;
        int8_16 b_4;
        int8_16 result_4;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_4 = load_int8_16(a);
            b_4 = load_int8_16(b);
            result_4 = a_4 * b_4;
            unload_int8_16(result_4, result);
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
void int8_16_mult(const int8* a, const int8* b, int8* result)
{
    int8_16 a_4 = load_int8_16(a);
    int8_16 b_4 = load_int8_16(b);
    int8_16 result_4 = a_4 * b_4;

    unload_int8_16(result_4, result);
}

inline
void int8_32_mult(const int8* a, const int8* b, int8* result)
{
    int8_32 a_8 = load_int8_32(a);
    int8_32 b_8 = load_int8_32(b);
    int8_32 result_8 = a_8 * b_8;

    unload_int8_32(result_8, result);
}

inline
void int8_64_mult(const int8* a, const int8* b, int8* result)
{
    int8_64 a_16 = load_int8_64(a);
    int8_64 b_16 = load_int8_64(b);
    int8_64 result_16 = a_16 * b_16;

    unload_int8_64(result_16, result);
}

inline
void simd_add(const int8* a, const int8* b, int8* result, int size, int steps)
{
    int i = 0;

    if (steps == 16) {
        int8_64 a_16;
        int8_64 b_16;
        int8_64 result_16;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_16 = load_int8_64(a);
            b_16 = load_int8_64(b);
            result_16 = a_16 + b_16;
            unload_int8_64(result_16, result);
       }
    } else if (steps == 8) {
        int8_32 a_8;
        int8_32 b_8;
        int8_32 result_8;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_8 = load_int8_32(a);
            b_8 = load_int8_32(b);
            result_8 = a_8 + b_8;
            unload_int8_32(result_8, result);
       }
    } else if (steps == 4) {
        int8_16 a_4;
        int8_16 b_4;
        int8_16 result_4;

        for (i = 0; i <= size - steps; i += steps) {
            ++a;
            ++b;
            ++result;

            a_4 = load_int8_16(a);
            b_4 = load_int8_16(b);
            result_4 = a_4 + b_4;
            unload_int8_16(result_4, result);
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
void int8_16_add(const int8* a, const int8* b, int8* result)
{
    int8_16 a_4 = load_int8_16(a);
    int8_16 b_4 = load_int8_16(b);
    int8_16 result_4 = a_4 + b_4;

    unload_int8_16(result_4, result);
}

inline
void int8_32_add(const int8* a, const int8* b, int8* result)
{
    int8_32 a_8 = load_int8_32(a);
    int8_32 b_8 = load_int8_32(b);
    int8_32 result_8 = a_8 + b_8;

    unload_int8_32(result_8, result);
}

inline
void int8_64_add(const int8* a, const int8* b, int8* result)
{
    int8_64 a_16 = load_int8_64(a);
    int8_64 b_16 = load_int8_64(b);
    int8_64 result_16 = a_16 + b_16;

    unload_int8_64(result_16, result);
}

// @todo add more operations like the one above "int8_16_mult()"

#endif