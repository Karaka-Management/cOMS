/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_INTRINSICS_H
#define TOS_STDLIB_INTRINSICS_H

#include <immintrin.h>
#include <xmmintrin.h>

#if __linux__
    #include <x86intrin.h>
    #include <x86gprintrin.h>
#endif

#include "Types.h"

inline f32 sqrt(f32 a) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(a))); }
inline f64 sqrt(f64 a)
{
    __m128d temp  =_mm_set_sd(a);

    return _mm_cvtsd_f64(_mm_sqrt_sd(temp, temp));
}

inline f32 rsqrt(f32 a) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(a))); }
inline f64 rsqrt(f64 a)
{
    __m128d temp  =_mm_set_sd(a);

    return _mm_cvtsd_f64(
        _mm_div_sd(
            _mm_set_sd(1.0),
            _mm_sqrt_sd(temp, temp)
        )
    );
}

inline f32 round(f32 a)
{
    return _mm_cvtss_f32(
        _mm_round_ss(_mm_setzero_ps(), _mm_set_ss(a), (_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)));
}

inline uint32 round_to_int(f32 a) { return (uint32) _mm_cvtss_si32(_mm_set_ss(a)); }

inline f32 floor(f32 a) { return _mm_cvtss_f32(_mm_floor_ss(_mm_setzero_ps(), _mm_set_ss(a))); }

inline f32 ceil(f32 a) { return _mm_cvtss_f32(_mm_ceil_ss(_mm_setzero_ps(), _mm_set_ss(a))); }

inline uint32 hash(uint64 a, uint64 b = 0)
{
    uint8 seed[16] = {
        0xaa, 0x9b, 0xbd, 0xb8, 0xa1, 0x98, 0xac, 0x3f, 0x1f, 0x94, 0x07, 0xb3, 0x8c, 0x27, 0x93, 0x69,
    };

    __m128i hash = _mm_set_epi64x(a, b);
    hash         = _mm_aesdec_si128(hash, _mm_loadu_si128((__m128i *) seed));
    hash         = _mm_aesdec_si128(hash, _mm_loadu_si128((__m128i *) seed));

    return _mm_extract_epi32(hash, 0);
}

inline void atomic_increment(int32* a, int32 b) {
    _aadd_i32(a, b);
}

inline void atomic_increment(int64* a, int64 b) {
    _aadd_i64(a, b);
}

inline void atomic_decrement(int32* a, int32 b) {
    _aadd_i32(a, -b);
}

inline void atomic_decrement(int64* a, int64 b) {
    _aadd_i64(a, -b);
}

#endif