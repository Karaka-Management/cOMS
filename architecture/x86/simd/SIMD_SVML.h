/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_SIMD_SVML_H
#define TOS_STDLIB_SIMD_SVML_H

#include <immintrin.h>
#include <xmmintrin.h>

#include "../../../stdlib/Types.h"

#if __linux__
    #include "math.h"

    inline __m128i _mm_div_epi32(__m128i a, __m128i b) {
        alignas(16) int32_t a_array[4], b_array[4], result[4];

        _mm_storeu_si128((__m128i*) a_array, a);
        _mm_storeu_si128((__m128i*) b_array, b);

        for (int32 i = 0; i < 4; ++i) {
            result[i] = a_array[i] / b_array[i];
        }

        return _mm_load_si128((__m128i*) result);
    }

    inline __m256i _mm256_div_epi32(__m256i a, __m256i b) {
        alignas(32) int32_t a_array[8], b_array[8], result[8];

        _mm256_storeu_si256((__m256i*) a_array, a);
        _mm256_storeu_si256((__m256i*) b_array, b);

        for (int32 i = 0; i < 8; ++i) {
            result[i] = a_array[i] / b_array[i];
        }

        return _mm256_load_si256((__m256i*) result);
    }

    inline __m512i _mm512_div_epi32(__m512i a, __m512i b) {
        alignas(64) int32_t a_array[16], b_array[16], result[16];

        _mm512_storeu_si512((__m512i*) a_array, a);
        _mm512_storeu_si512((__m512i*) b_array, b);

        for (int32 i = 0; i < 16; ++i) {
            result[i] = a_array[i] / b_array[i];
        }

        return _mm512_load_si512((__m512i*) result);
    }

    inline __m128 _mm_sin_ps(__m128 a) {
        alignas(16) f32 a_array[4], result[4];
        _mm_storeu_ps(a_array, a);
        for (int32 i = 0; i < 4; ++i) {
            result[i] = sinf(a_array[i]);
        }
        return _mm_load_ps(result);
    }

    inline __m128 _mm_cos_ps(__m128 a) {
        alignas(16) f32 a_array[4], result[4];
        _mm_storeu_ps(a_array, a);
        for (int32 i = 0; i < 4; ++i) {
            result[i] = cosf(a_array[i]);
        }
        return _mm_load_ps(result);
    }

    inline __m128 _mm_asin_ps(__m128 a) {
        alignas(16) f32 a_array[4], result[4];
        _mm_storeu_ps(a_array, a);
        for (int32 i = 0; i < 4; ++i) {
            result[i] = asinf(a_array[i]);
        }
        return _mm_load_ps(result);
    }

    inline __m128 _mm_acos_ps(__m128 a) {
        alignas(16) f32 a_array[4], result[4];
        _mm_storeu_ps(a_array, a);
        for (int32 i = 0; i < 4; ++i) {
            result[i] = acosf(a_array[i]);
        }
        return _mm_load_ps(result);
    }

    inline __m256 _mm256_sin_ps(__m256 a) {
        alignas(32) f32 a_array[8], result[8];
        _mm256_storeu_ps(a_array, a);
        for (int32 i = 0; i < 8; ++i) {
            result[i] = sinf(a_array[i]);
        }
        return _mm256_load_ps(result);
    }

    inline __m256 _mm256_cos_ps(__m256 a) {
        alignas(32) f32 a_array[8], result[8];
        _mm256_storeu_ps(a_array, a);
        for (int32 i = 0; i < 8; ++i) {
            result[i] = cosf(a_array[i]);
        }
        return _mm256_load_ps(result);
    }

    inline __m256 _mm256_asin_ps(__m256 a) {
        alignas(32) f32 a_array[8], result[8];
        _mm256_storeu_ps(a_array, a);
        for (int32 i = 0; i < 8; ++i) {
            result[i] = asinf(a_array[i]);
        }
        return _mm256_load_ps(result);
    }

    inline __m256 _mm256_acos_ps(__m256 a) {
        alignas(32) f32 a_array[8], result[8];
        _mm256_storeu_ps(a_array, a);
        for (int32 i = 0; i < 16; ++i) {
            result[i] = acosf(a_array[i]);
        }
        return _mm256_load_ps(result);
    }

    inline __m512 _mm512_sin_ps(__m512 a) {
        alignas(64) f32 a_array[8], result[8];
        _mm512_storeu_ps(a_array, a);
        for (int32 i = 0; i < 16; ++i) {
            result[i] = sinf(a_array[i]);
        }
        return _mm512_load_ps(result);
    }

    inline __m512 _mm512_cos_ps(__m512 a) {
        alignas(64) f32 a_array[8], result[8];
        _mm512_storeu_ps(a_array, a);
        for (int32 i = 0; i < 16; ++i) {
            result[i] = cosf(a_array[i]);
        }
        return _mm512_load_ps(result);
    }

    inline __m512 _mm512_asin_ps(__m512 a) {
        alignas(64) f32 a_array[8], result[8];
        _mm512_storeu_ps(a_array, a);
        for (int32 i = 0; i < 16; ++i) {
            result[i] = asinf(a_array[i]);
        }
        return _mm512_load_ps(result);
    }

    inline __m512 _mm512_acos_ps(__m512 a) {
        alignas(64) f32 a_array[16], result[16];
        _mm512_storeu_ps(a_array, a);
        for (int32 i = 0; i < 16; ++i) {
            result[i] = acosf(a_array[i]);
        }
        return _mm512_load_ps(result);
    }
#endif

#endif