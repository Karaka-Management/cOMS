/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef STDLIB_SIMD_I32_H
#define STDLIB_SIMD_I32_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../Types.h"
#include "SIMD_F32.h"

namespace Stdlib::SIMD
{
    struct int32_4_simd {
        union {
            __m128i s;
            int32 v[4];
        };
    };

    struct int32_8_simd {
        union {
            __m256i s;
            int32 v[8];
        };
    };

    struct int32_16_simd {
        union {
            __m512i s;
            int32 v[16];
        };
    };

    inline int32_4_simd load_int32_4_simd(int32 *mem)
    {
        int32_4_simd simd;
        simd.s = _mm_loadu_epi32(mem);

        return simd;
    }

    inline int32_4_simd init_int32_4_simd(int32 *mem)
    {
        int32_4_simd simd;
        simd.s = _mm_set_epi32(mem[0], mem[1], mem[2], mem[3]);

        return simd;
    }

    inline void unload_int32_4_simd(int32_4_simd a, int32 *array) { _mm_store_epi32(array, a.s); }

    inline int32_8_simd load_int32_8_simd(int32 *mem)
    {
        int32_8_simd simd;
        simd.s = _mm256_loadu_epi32(mem);

        return simd;
    }

    inline int32_8_simd init_int32_8_simd(int32 *mem)
    {
        int32_8_simd simd;
        simd.s = _mm256_set_epi32(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]);

        return simd;
    }

    inline void unload_int32_8_simd(int32_8_simd a, int32 *array) { _mm256_store_epi32(array, a.s); }

    inline int32_16_simd load_int32_16_simd(int32 *mem)
    {
        int32_16_simd simd;
        simd.s = _mm512_loadu_epi32(mem);

        return simd;
    }

    inline int32_16_simd init_int32_16_simd(int32 *mem)
    {
        int32_16_simd simd;
        simd.s = _mm512_set_epi32(mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9],
                                  mem[10], mem[11], mem[12], mem[13], mem[14], mem[15]);

        return simd;
    }

    inline void unload_int32_16_simd(int32_16_simd a, int32 *array) { _mm512_store_epi32(array, a.s); }

    inline int32_4_simd init_zero_int32_4_simd()
    {
        int32_4_simd simd;
        simd.s = _mm_setzero_si128();

        return simd;
    }

    inline int32_8_simd init_zero_int32_8_simd()
    {
        int32_8_simd simd;
        simd.s = _mm256_setzero_si256();

        return simd;
    }

    inline int32_16_simd init_zero_int32_16_simd()
    {
        int32_16_simd simd;
        simd.s = _mm512_setzero_epi32();

        return simd;
    }

    inline int32_4_simd operator+(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_add_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator+(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_add_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator+(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_add_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator-(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_sub_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator-(int32_4_simd a) { return init_zero_int32_4_simd() - a; }

    inline int32_8_simd operator-(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_sub_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator-(int32_8_simd a) { return init_zero_int32_8_simd() - a; }

    inline int32_16_simd operator-(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_sub_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator-(int32_16_simd a) { return init_zero_int32_16_simd() - a; }

    inline int32_4_simd operator*(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_mul_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator*(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_mul_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator*(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mul_epi32(a.s, b.s);

        return simd;
    }

    inline Stdlib::SIMD::f32_4_simd operator/(int32_4_simd a, int32_4_simd b)
    {
        Stdlib::SIMD::f32_4_simd simd;
        simd.s = _mm_div_ps(a.s, b.s);

        return simd;
    }

    inline Stdlib::SIMD::f32_8_simd operator/(int32_8_simd a, int32_8_simd b)
    {
        Stdlib::SIMD::f32_8_simd simd;
        simd.s = _mm256_div_ps(a.s, b.s);

        return simd;
    }

    inline Stdlib::SIMD::f32_16_simd operator/(int32_16_simd a, int32_16_simd b)
    {
        Stdlib::SIMD::f32_16_simd simd;
        simd.s = _mm512_div_ps(a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator^(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_xor_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator^(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_xor_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator^(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_xor_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd &operator-=(int32_4_simd &a, int32_4_simd b)
    {
        a = a - b;

        return a;
    }

    inline int32_8_simd &operator-=(int32_8_simd &a, int32_8_simd b)
    {
        a = a - b;

        return a;
    }

    inline int32_16_simd &operator-=(int32_16_simd &a, int32_16_simd b)
    {
        a = a - b;

        return a;
    }

    inline int32_4_simd &operator+=(int32_4_simd &a, int32_4_simd b)
    {
        a = a + b;

        return a;
    }

    inline int32_8_simd &operator+=(int32_8_simd &a, int32_8_simd b)
    {
        a = a + b;

        return a;
    }

    inline int32_16_simd &operator+=(int32_16_simd &a, int32_16_simd b)
    {
        a = a + b;

        return a;
    }

    inline int32_4_simd &operator*=(int32_4_simd &a, int32_4_simd b)
    {
        a = a * b;

        return a;
    }

    inline int32_8_simd &operator*=(int32_8_simd &a, int32_8_simd b)
    {
        a = a * b;

        return a;
    }

    inline int32_16_simd &operator*=(int32_16_simd &a, int32_16_simd b)
    {
        a = a * b;

        return a;
    }

    inline int32_4_simd &operator/=(int32_4_simd &a, int32_4_simd b)
    {
        a.s = _mm_cvtps_epi32((a / b).s);

        return a;
    }

    inline int32_8_simd &operator/=(int32_8_simd &a, int32_8_simd b)
    {
        a.s = _mm256_cvtps_epi32((a / b).s);

        return a;
    }

    inline int32_16_simd &operator/=(int32_16_simd &a, int32_16_simd b)
    {
        a.s = _mm512_cvtps_epi32((a / b).s);

        return a;
    }

    inline int32_4_simd &operator^=(int32_4_simd &a, int32_4_simd b)
    {
        a = a ^ b;

        return a;
    }

    inline int32_8_simd &operator^=(int32_8_simd &a, int32_8_simd b)
    {
        a = a ^ b;

        return a;
    }

    inline int32_16_simd &operator^=(int32_16_simd &a, int32_16_simd b)
    {
        a = a ^ b;

        return a;
    }

    inline int32_4_simd operator<(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_cmplt_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator<(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_xor_si256(_mm256_cmpgt_epi32(a.s, b.s), _mm256_set1_epi32(-1));

        return simd;
    }

    inline int32_16_simd operator<(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mask_blend_epi32(_mm512_cmplt_epi32_mask(a.s, b.s), a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator<=(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_andnot_si128(_mm_cmplt_epi32(b.s, a.s), _mm_set1_epi32(-1));

        return simd;
    }

    inline int32_8_simd operator<=(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi32(a.s, b.s), _mm256_set1_epi32(-1));

        return simd;
    }

    inline int32_16_simd operator<=(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mask_blend_epi32(_mm512_knot(_mm512_cmpgt_epi32_mask(b.s, a.s)), b.s, a.s);

        return simd;
    }

    inline int32_4_simd operator>(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_cmpgt_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator>(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_cmpgt_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator>(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mask_blend_ps(_mm512_cmpgt_epi32_mask(a.s, b.s), a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator>=(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_andnot_si128(_mm_cmplt_epi32(a.s, b.s), _mm_set1_epi32(-1));

        return simd;
    }

    inline int32_8_simd operator>=(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_andnot_si256(_mm256_cmpgt_epi32(b.s, a.s), _mm256_set1_epi32(-1));

        return simd;
    }

    inline int32_16_simd operator>=(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mask_blend_ps(_mm512_cmpge_epi32_mask(a.s, b.s), a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator==(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_cmpeq_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator==(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_cmpeq_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator==(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mask_blend_ps(_mm512_cmpeq_epi32_mask(a.s, b.s), a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator!=(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_cmpneq_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator!=(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_cmp_epi32(a.s, b.s, _CMP_NEQ_OQ);

        return simd;
    }

    inline int32_16_simd operator!=(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_mask_mov_epi32(_mm512_setzero_epi32(), _mm512_cmp_ps_mask(a.s, b.s, _CMP_NEQ_OQ),
                                       _mm512_set1_epi32(1.0f));

        return simd;
    }

    inline int32_4_simd operator&(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_and_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator&(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_and_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator&(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_and_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd operator|(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_or_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd operator|(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_or_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd operator|(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_or_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd &operator&=(int32_4_simd &a, int32_4_simd b)
    {
        a = a & b;

        return a;
    }

    inline int32_8_simd &operator&=(int32_8_simd &a, int32_8_simd b)
    {
        a = a & b;

        return a;
    }

    inline int32_16_simd &operator&=(int32_16_simd &a, int32_16_simd b)
    {
        a = a & b;

        return a;
    }

    inline int32_4_simd &operator|=(int32_4_simd &a, int32_4_simd b)
    {
        a = a | b;

        return a;
    }

    inline int32_8_simd &operator|=(int32_8_simd &a, int32_8_simd b)
    {
        a = a | b;

        return a;
    }

    inline int32_16_simd &operator|=(int32_16_simd &a, int32_16_simd b)
    {
        a = a | b;

        return a;
    }

    inline int32_4_simd abs(int32_4_simd a)
    {
        unsigned int unsigned_mask = (unsigned int) (1 << 31);
        __m128 mask                = _mm_set1_epi32(*(float *) &unsigned_mask);

        int32_4_simd simd;
        simd.s = _mm_and_epi32(a.s, mask);

        return simd;
    }

    inline int32_8_simd abs(int32_8_simd a)
    {
        unsigned int unsigned_mask = (unsigned int) (1 << 31);
        __m256 mask                = _mm256_set1_epi32(*(float *) &unsigned_mask);

        int32_8_simd simd;
        simd.s = _mm256_and_epi32(a.s, mask);

        return simd;
    }

    inline int32_16_simd abs(int32_16_simd a)
    {
        unsigned int unsigned_mask = (unsigned int) (1 << 31);
        __m512 mask                = _mm512_set1_epi32(*(float *) &unsigned_mask);

        int32_16_simd simd;
        simd.s = _mm512_and_epi32(a.s, mask);

        return simd;
    }

    inline int32_4_simd min(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_min_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd min(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_min_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd min(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_min_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd max(int32_4_simd a, int32_4_simd b)
    {
        int32_4_simd simd;
        simd.s = _mm_max_epi32(a.s, b.s);

        return simd;
    }

    inline int32_8_simd max(int32_8_simd a, int32_8_simd b)
    {
        int32_8_simd simd;
        simd.s = _mm256_max_epi32(a.s, b.s);

        return simd;
    }

    inline int32_16_simd max(int32_16_simd a, int32_16_simd b)
    {
        int32_16_simd simd;
        simd.s = _mm512_max_epi32(a.s, b.s);

        return simd;
    }

    inline int32_4_simd sign(int32_4_simd a)
    {
        unsigned int umask = (unsigned int) (1 << 31);
        __m128 mask        = _mm_set1_epi32(*(float *) &umask);

        int32_4_simd signBit;
        signBit.s = _mm_and_epi32(a.s, mask);

        int32_4_simd b;
        b.s = _mm_set1_epi32(1.0f);

        int32_4_simd simd = b | signBit;

        return simd;
    }

    inline int32_8_simd sign(int32_8_simd a)
    {
        unsigned int umask = (unsigned int) (1 << 31);
        __m256 mask        = _mm256_set1_epi32(*(float *) &umask);

        int32_8_simd signBit;
        signBit.s = _mm256_and_epi32(a.s, mask);

        int32_8_simd b;
        b.s = _mm256_set1_epi32(1.0f);

        int32_8_simd simd = b | signBit;

        return simd;
    }

    inline int32_16_simd sign(int32_16_simd a)
    {
        unsigned int umask = (unsigned int) (1 << 31);
        __m512 mask        = _mm512_set1_epi32(*(float *) &umask);

        int32_16_simd signBit;
        signBit.s = _mm512_and_epi32(a.s, mask);

        int32_16_simd b;
        b.s = _mm512_set1_epi32(1.0f);

        int32_16_simd simd = b | signBit;

        return simd;
    }

    inline int32_4_simd floor(int32_4_simd a)
    {
        int32_4_simd simd;
        simd.s = _mm_floor_epi32(a.s);

        return simd;
    }

    inline int32_8_simd floor(int32_8_simd a)
    {
        int32_8_simd simd;
        simd.s = _mm256_floor_epi32(a.s);

        return simd;
    }

    inline int32_16_simd floor(int32_16_simd a)
    {
        int32_16_simd simd;
        simd.s = _mm512_floor_epi32(a.s);

        return simd;
    }

    inline int32_4_simd ceil(int32_4_simd a)
    {
        int32_4_simd simd;
        simd.s = _mm_ceil_epi32(a.s);

        return simd;
    }

    inline int32_8_simd ceil(int32_8_simd a)
    {
        int32_8_simd simd;
        simd.s = _mm256_ceil_epi32(a.s);

        return simd;
    }

    inline int32_16_simd ceil(int32_16_simd a)
    {
        int32_16_simd simd;
        simd.s = _mm512_ceil_epi32(a.s);

        return simd;
    }

    inline int32_4_simd sqrt(int32_4_simd a)
    {
        int32_4_simd simd;
        simd.s = _mm_sqrt_epi32(a.s);

        return simd;
    }

    inline int32_8_simd sqrt(int32_8_simd a)
    {
        int32_8_simd simd;
        simd.s = _mm256_sqrt_epi32(a.s);

        return simd;
    }

    inline int32_16_simd sqrt(int32_16_simd a)
    {
        int32_16_simd simd;
        simd.s = _mm512_sqrt_epi32(a.s);

        return simd;
    }

    inline int32_4_simd sqrt_inv_approx(int32_4_simd a)
    {
        int32_4_simd simd;
        simd.s = _mm_rsqrt_epi32(a.s);

        return simd;
    }

    inline int32_8_simd sqrt_inv_approx(int32_8_simd a)
    {
        int32_8_simd simd;
        simd.s = _mm256_rsqrt_epi32(a.s);

        return simd;
    }

    inline int32_16_simd sqrt_inv_approx(int32_16_simd a)
    {
        int32_16_simd simd;
        simd.s = _mm512_rsqrt14_epi32(a.s);

        return simd;
    }

    inline int32_4_simd one_over_approx(int32_4_simd a)
    {
        int32_4_simd simd;
        simd.s = _mm_rcp_epi32(a.s);

        return simd;
    }

    inline int32_8_simd one_over_approx(int32_8_simd a)
    {
        int32_8_simd simd;
        simd.s = _mm256_rcp_epi32(a.s);

        return simd;
    }

    inline int32_16_simd one_over_approx(int32_16_simd a)
    {
        int32_16_simd simd;
        simd.s = _mm512_rcp14_epi32(a.s);

        return simd;
    }

    inline int32_4_simd clamp(int32_4_simd min_value, int32_4_simd a, int32_4_simd max_value)
    {
        return min(max(a, min_value), max_value);
    }

    inline int32_8_simd clamp(int32_8_simd min_value, int32_8_simd a, int32_8_simd max_value)
    {
        return min(max(a, min_value), max_value);
    }

    inline int32_16_simd clamp(int32_16_simd min_value, int32_16_simd a, int32_16_simd max_value)
    {
        return min(max(a, min_value), max_value);
    }

    inline int32 which_true(int32_4_simd a)
    {
        int32 which_true = _mm_movemask_epi32(a.s);

        return which_true;
    }

    inline int32 which_true(int32_8_simd a)
    {
        int32 which_true = _mm256_movemask_epi32(a.s);

        return which_true;
    }

    inline int32 which_true(int32_16_simd a)
    {
        int32 which_true = _mm512_movepi32_mask(_mm512_castps_si512(a.s));

        return which_true;
    }

    inline bool any_true(int32_4_simd a)
    {
        bool is_any_true = _mm_movemask_epi32(a.s) > 0;

        return is_any_true;
    }

    inline bool any_true(int32_8_simd a)
    {
        bool is_any_true = _mm256_movemask_epi32(a.s) > 0;

        return is_any_true;
    }

    inline bool any_true(int32_16_simd a)
    {
        bool is_any_true = _mm512_movepi32_mask(_mm512_castps_si512(a.s)) > 0;

        return is_any_true;
    }

    inline bool all_true(int32_4_simd a)
    {
        bool is_true = (_mm_movemask_epi32(a.s) == 15);

        return is_true;
    }

    inline bool all_true(int32_8_simd a)
    {
        bool is_true = (_mm256_movemask_epi32(a.s) == 255);

        return is_true;
    }

    inline bool all_true(int32_16_simd a)
    {
        bool is_true = (_mm512_movepi32_mask(_mm512_castps_si512(a.s)) == 65535);

        return is_true;
    }

    inline bool all_false(int32_4_simd a)
    {
        bool is_false = (_mm_movemask_epi32(a.s) == 0);

        return is_false;
    }

    inline bool all_false(int32_8_simd a)
    {
        bool is_false = (_mm256_movemask_epi32(a.s) == 0);

        return is_false;
    }

    inline bool all_false(int32_16_simd a)
    {
        // @todo This can be optimized (requires also changes in the comparison functions return)
        bool is_false = (_mm512_movepi32_mask(_mm512_castps_si512(a.s)) == 0);

        return is_false;
    }
} // namespace Stdlib::SIMD

#endif
