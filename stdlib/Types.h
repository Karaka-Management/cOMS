/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_TYPES_H
#define TOS_TYPES_H

#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint16_t f16;
typedef float f32;
typedef double f64;

typedef unsigned char byte;

#define KILOBYTE 1024
#define MEGABYTE 1048576
#define GIGABYTE 1073741824

#define MAX_BYTE 0xFF
#define MAX_INT16 0xFFFF
#define MAX_INT32 0xFFFFFFFF

#define internal static // only allows local "file" access
#define local_persist static
#define global_persist static

#define HALF_FLOAT_SIGN_MASK   0x8000
#define HALF_FLOAT_EXP_MASK    0x7C00
#define HALF_FLOAT_FRAC_MASK   0x03FF

#define HALF_FLOAT_EXP_SHIFT   10
#define HALF_FLOAT_EXP_BIAS    15

#define FLOAT32_SIGN_MASK      0x80000000
#define FLOAT32_EXP_MASK       0x7F800000
#define FLOAT32_FRAC_MASK      0x007FFFFF

#define FLOAT32_EXP_SHIFT      23
#define FLOAT32_EXP_BIAS       127

uint16 float_to_f16(float f) {
    uint32_t f_bits = *((uint32_t*)&f);
    uint16_t f16_bits = 0;

    // Extract sign, exponent, and fraction from float
    uint16_t sign = (f_bits & FLOAT32_SIGN_MASK) >> 16;
    int32_t exponent = (int32_t) ((f_bits & FLOAT32_EXP_MASK) >> FLOAT32_EXP_SHIFT) - FLOAT32_EXP_BIAS + HALF_FLOAT_EXP_BIAS;
    uint32_t fraction = (f_bits & FLOAT32_FRAC_MASK) >> (FLOAT32_EXP_SHIFT - HALF_FLOAT_EXP_SHIFT);

    if (exponent <= 0) {
        if (exponent < -10) {
            fraction = 0;
        } else {
            fraction = (fraction | 0x0400) >> (1 - exponent);
        }
        exponent = 0;
    } else if (exponent >= 0x1F) {
        exponent = 0x1F;
        fraction = 0;
    }

    f16_bits = (uint16_t) (sign | (exponent << HALF_FLOAT_EXP_SHIFT) | (fraction & HALF_FLOAT_FRAC_MASK));

    return f16_bits;
}

float f16_to_float(f16 f) {
    uint32_t f_bits = 0;

    uint32_t sign = (f & HALF_FLOAT_SIGN_MASK) << 16;
    int32_t exponent = (f & HALF_FLOAT_EXP_MASK) >> HALF_FLOAT_EXP_SHIFT;
    uint32_t fraction = (f & HALF_FLOAT_FRAC_MASK) << (FLOAT32_EXP_SHIFT - HALF_FLOAT_EXP_SHIFT);

    if (exponent == 0) {
        if (fraction != 0) {
            exponent = 1;
            while ((fraction & (1 << FLOAT32_EXP_SHIFT)) == 0) {
                fraction <<= 1;
                --exponent;
            }
            fraction &= ~FLOAT32_EXP_MASK;
        }
    } else if (exponent == 0x1F) {
        exponent = 0xFF;
    } else {
        exponent += FLOAT32_EXP_BIAS - HALF_FLOAT_EXP_BIAS;
    }

    f_bits = sign | (exponent << FLOAT32_EXP_SHIFT) | fraction;

    return *((float*)&f_bits);
}

#endif
