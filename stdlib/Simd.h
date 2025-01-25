/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_SIMD_H
#define TOS_STDLIB_SIMD_H

#if __aarch64__

#else
    // @todo Should get moved to architecture/x86/simd directory
    #include "simd/SIMD_F32.h"
    #include "simd/SIMD_F64.h"
    #include "simd/SIMD_I8.h"
    #include "simd/SIMD_I16.h"
    #include "simd/SIMD_I32.h"
    #include "simd/SIMD_I64.h"
    #include "simd/SIMD_SVML.h"
#endif

#endif