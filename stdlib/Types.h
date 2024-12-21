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

#ifdef _MSC_VER
    #include <windows.h>

    #define PACKED_STRUCT  __pragma(pack(push, 1))
    #define UNPACKED_STRUCT __pragma(pack(pop))
    typedef SSIZE_T ssize_t;

    #define UNREACHABLE() __assume(0)
#else
    #define PACKED_STRUCT  __attribute__((__packed__))
    #define UNPACKED_STRUCT ((void) 0)

    #define UNREACHABLE() __builtin_unreachable()
#endif

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

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
typedef char sbyte;

typedef uintptr_t umm;
typedef intptr_t smm;

#define KILOBYTE 1024
#define MEGABYTE 1048576
#define GIGABYTE 1073741824

#define MAX_BYTE 0xFF
#define MAX_UINT16 0xFFFF
#define MAX_UINT32 0xFFFFFFFF
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

#define MAX_CHAR 0x7F
#define MAX_INT16 0x7FFF
#define MAX_INT32 0x7FFFFFFF
#define MAX_INT64 0x7FFFFFFFFFFFFFFF

#define MIN_CHAR 0x80
#define MIN_INT16 0x8000
#define MIN_INT32 0x80000000
#define MIN_INT64 0x8000000000000000

#define SEC_MILLI 1000
#define MILLI_MICRO 1000
#define SEC_MICRO 1000000

#define MHZ 1000000
#define GHZ 1000000000

#define internal static // only allows local "file" access
#define local_persist static
#define global_persist static

struct v3_byte {
    union {
        struct {
            byte x, y, z;
        };

        struct {
            byte r, g, b;
        };

        byte v[3];
    };
};

struct v4_byte {
    union {
        struct {
            byte x, y, z, w;
        };

        struct {
            byte r, g, b, a;
        };

        union {
            byte v[4];
            uint32 val;
        };
    };
};

struct v2_int32 {
    union {
        struct {
            int32 x, y;
        };

        struct {
            int32 width, height;
        };

        int32 v[2];
    };
};

struct v3_int32 {
    union {
        struct {
            int32 x, y, z;
        };

        struct {
            int32 r, g, b;
        };

        int32 v[3];
    };
};

struct v4_int32 {
    union {
        struct {
            int32 x, y;

            union {
                int32 z, width;
            };

            union {
                int32 w, height;
            };
        };

        int32 v[4];
    };
};

struct v2_int64 {
    union {
        struct {
            int64 x, y;
        };

        int64 v[2];
    };
};

struct v3_int64 {
    union {
        struct {
            int64 x, y, z;
        };

        struct {
            int64 r, g, b;
        };

        int64 v[3];
    };
};

struct v4_int64 {
    union {
        struct {
            int64 x, y, z, w;
        };

        int64 v[4];
    };
};

struct v2_f32 {
    union {
        struct {
            f32 x, y;
        };

        f32 v[2];
    };
};

struct v3_f32 {
    union {
        struct {
            f32 x, y, z;
        };

        struct {
            f32 r, g, b;
        };

        struct {
            f32 pitch, yaw, roll;
        };

        struct {
            f32 u, v, w;
        };

        f32 vec[3];
    };
};

struct v4_f32 {
    union {
        struct {
            f32 x, y;

            union {
                struct {
                    f32 z, w;
                };

                struct {
                    f32 width, height;
                };
            };
        };

        struct {
            f32 x1, y1, x2, y2;
        };

        struct {
            f32 r, g, b, a;
        };

        f32 vec[4];
    };
};

struct v2_f64 {
    union {
        struct {
            f64 x;
            f64 y;
        };

        f64 v[2];
    };
};

struct v3_f64 {
    union {
        struct {
            f64 x, y, z;
        };

        struct {
            f64 r, g, b;
        };

        f64 v[3];
    };
};

struct v4_f64 {
    union {
        struct {
            f64 x;
            f64 y;
            f64 z;
            f64 w;
        };

        f64 v[4];
    };
};

struct m_int32 {
    int32 *e;
    size_t m, n;
};

struct m_int64 {
    int64 *e;
    size_t m, n;
};

struct m_f32 {
    f32 *e;
    size_t m, n;
};

struct m_f64 {
    f64 *e;
    size_t m, n;
};

#define HALF_FLOAT_SIGN_MASK 0x8000
#define HALF_FLOAT_EXP_MASK 0x7C00
#define HALF_FLOAT_FRAC_MASK 0x03FF

#define HALF_FLOAT_EXP_SHIFT 10
#define HALF_FLOAT_EXP_BIAS 15

#define FLOAT32_SIGN_MASK 0x80000000
#define FLOAT32_EXP_MASK 0x7F800000
#define FLOAT32_FRAC_MASK 0x007FFFFF

#define FLOAT32_EXP_SHIFT 23
#define FLOAT32_EXP_BIAS 127

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

    uint32_t f_bits = sign | (exponent << FLOAT32_EXP_SHIFT) | fraction;

    return *((float *) &f_bits);
}

#endif
