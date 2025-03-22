/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_STDLIB_SIMD_SVML_H
#define COMS_STDLIB_SIMD_SVML_H

#ifdef MACRO_CPU_FEATURE_SSE42
    #include "SIMD_SVML_SSE.h"
#endif

#ifdef MACRO_CPU_FEATURE_AVX2
    #include "SIMD_SVML_AVX2.h"
#endif

#ifdef MACRO_CPU_FEATURE_AVX512
    #include "SIMD_SVML_AVX512.h"
#endif

#endif