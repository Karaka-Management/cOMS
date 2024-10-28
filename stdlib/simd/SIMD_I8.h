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
#include "SIMD_F32.h"
#include "SIMD_I32.h"

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
    simd.s = _mm_load_si128((__m128i *) mem);

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
    simd.s = _mm256_load_si256((__m256i *) mem);

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
    simd.s = _mm512_load_si512((__m512i *) mem);

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

inline
f32_4 int8_16_to_f32_4(int8_16 a)
{
    f32_4 result;
    result.s = _mm_cvtepi32_ps(a.s);

    return result;
}

inline
f32_8 int8_16_to_f32_8(int8_16 a)
{
    f32_8 result;
    result.s = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(a.s));

    return result;
}

inline
f32_16 int8_16_to_f32_16(int8_16 a)
{
    f32_16 result;
    result.s = _mm512_cvtepi32_ps(_mm512_cvtepu8_epi32(a.s));

    return result;
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
    simd.s = _mm512_mask_blend_epi8(_mm512_cmple_epi8_mask(a.s, b.s), b.s, a.s);

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

inline int32 which_true(int8_16 a)
{
    int32 which_true = _mm_movemask_epi8(a.s);

    return which_true;
}

inline int32 which_true(int8_32 a)
{
    int32 which_true = _mm256_movemask_epi8(a.s);

    return which_true;
}

inline int64 which_true(int8_64 a)
{
    int64 which_true = _mm512_movepi8_mask(a.s);

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

/*
inline
f32 simd_mult(const int8* a, f32 b, int32 size, int32 steps)
{
    if (steps == 16) {
        __m512i a_16 = _mm512_load_si512((__m512i *) a);
        __m512 af_16 = _mm512_cvtepi32_ps(a_16);
        __m512 b_16 = _mm512_set1_ps(b);

        __m512 result = _mm512_mul_ps(af_16, b_16);
    } else if (steps == 8) {
        __m256i a_8 = _mm256_load_si256((__m256i *) a);
        __m256 af_8 = _mm256_cvtepi32_ps(a_8);
        __m256 b_8 = _mm256_set1_ps(b);

        __m256 result = _mm256_mul_ps(af_8, b_8);
    } else if (steps == 4) {
        __m128i a_4 = _mm_load_si128((__m128i *) a);
        __m128 af_4 = _mm_cvtepi32_ps(a_4);
        __m128 b_4 = _mm_set1_ps(b);

        __m128 result = _mm_mul_ps(af_4, b_4);
    } else {

    }
}
*/

bool simd_compare_64(const byte* a, const byte* b)
{
    __m256i chunk1_a = _mm256_load_si256((__m256i*) a);
    __m256i chunk1_b = _mm256_load_si256((__m256i*) b);

    __m256i chunk2_a = _mm256_load_si256((__m256i*) (a + 32));
    __m256i chunk2_b = _mm256_load_si256((__m256i*) (b + 32));

    __m256i result1 = _mm256_cmpeq_epi8(chunk1_a, chunk1_b);
    __m256i result2 = _mm256_cmpeq_epi8(chunk2_a, chunk2_b);

    __m256i combined = _mm256_and_si256(result1, result2);

    return _mm256_testc_si256(combined, _mm256_set1_epi8(-1)) != 1;
}

int simd_compare(const byte* a, const byte* b, uint32 size, uint32 steps = 8) {
    int32 i = 0;

    if (steps == 16) {
        if (size >= 128) {
            __m512i a_16;
            __m512i b_16;
            __mmask64 result_mask;

            for (; i <= size - 64; i += 64) {  // 64 bytes per iteration
                a_16 = _mm512_load_si512((__m512i*) a);
                b_16 = _mm512_load_si512((__m512i*) b);

                result_mask = _mm512_cmpeq_epi8_mask(a_16, b_16);

                if (result_mask != 0xFFFFFFFFFFFFFFFF) {
                    return false;
                }

                a += 64;
                b += 64;
            }
        }

        if (size - i >= 64) {
            return simd_compare(a, b, size - i, 8);
        } else if (size - i >= 32) {
            return simd_compare(a, b, size - i, 4);
        }
    } else if (steps == 8) {
        if (size >= 64) {
            __m256i a_8;
            __m256i b_8;
            __m256i result_8;

            for (; i <= size - steps; i += steps) {
                a_8 = _mm256_load_si256((__m256i*) a);
                b_8 = _mm256_load_si256((__m256i*) b);

                result_8 = _mm256_cmpeq_epi8(a_8, b_8);

                if (_mm256_testc_si256(result_8, _mm256_set1_epi8(-1)) != 1) {
                    return false;
                }

                a += steps;
                b += steps;
            }
        }

        if (size - i >= 32) {
            return simd_compare(a, b, size - i, 4);
        }
    } else if (steps == 4) {
        if (size >= 16) {
            __m128i a_4;
            __m128i b_4;
            __m128i result_4;

            for (; i <= size - steps; i += steps) {
                a_4 = _mm_load_si128((__m128i*) a);
                b_4 = _mm_load_si128((__m128i*) b);

                result_4 = _mm_cmpeq_epi8(a_4, b_4);

                if (_mm_movemask_epi8(result_4) != 0xFFFF) {
                    return false;
                }

                a += steps;
                b += steps;
            }
        }
    }

    for (; i < size; ++i) {
        if (*a++ != *b++) {
            return false;
        }
    }

    return true;
}

#endif