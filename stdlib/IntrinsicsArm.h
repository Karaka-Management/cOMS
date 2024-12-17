/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_INTRINSICS_ARM_H
#define TOS_STDLIB_INTRINSICS_ARM_H

#include <arm_sve.h>

inline float oms_sqrt(float a) {
    svfloat32_t input = svdup_f32(a);
    svfloat32_t result = svsqrt_f32(input);

    return svget1_f32(result);
}

inline double oms_sqrt(double a) {
    svfloat64_t input = svdup_f64(a);
    svfloat64_t result = svsqrt_f64(input);

    return svget1_f64(result);
}

inline float oms_rsqrt(float a) {
    svfloat32_t input = svdup_f32(a);
    svfloat32_t result = svrsqrte_f32(input);

    return svget1_f32(result);
}

inline double oms_rsqrt(double a) {
    svfloat64_t input = svdup_f64(a);
    svfloat64_t result = svrsqrte_f64(input);

    return svget1_f64(result);
}

inline float oms_round(float a) {
    svfloat32_t input = svdup_f32(a);
    svfloat32_t result = svrndn_f32(input);

    return svget1_f32(result);
}

inline uint32_t round_to_int(float a) {
    svfloat32_t input = svdup_f32(a);
    svint32_t result = svcvtn_f32_s32(input, SVE_32B);

    return svget1_s32(result);
}

inline float oms_floor(float a) {
    svfloat32_t input = svdup_f32(a);
    svfloat32_t result = svfloor_f32(input);

    return svget1_f32(result);
}

inline float oms_ceil(float a) {
    svfloat32_t input = svdup_f32(a);
    svfloat32_t result = svceil_f32(input);

    return svget1_f32(result);
}

#endif