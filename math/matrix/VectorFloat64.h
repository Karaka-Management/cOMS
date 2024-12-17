/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MATH_MATRIX_VECTOR_FLOAT64_H
#define TOS_MATH_MATRIX_VECTOR_FLOAT64_H

#include "../../utils/MathUtils.h"

#if __aarch64__
    #include "../../../GameEngine/stdlib/sve/SVE_F64.h"
#else
    #include "../../../GameEngine/stdlib/simd/SIMD_F64.h"
#endif

#endif