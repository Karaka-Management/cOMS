/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef STDLIB_MATHTYPES_H
#define STDLIB_MATHTYPES_H

#include <stdint.h>
#include <stdlib.h>

// @todo check Vectors, we can simplify this!!!

// int32_t vectors
typedef union {
    struct {
        int32_t x, y;
    };

    int32_t e[2];
} i2;

typedef union {
    struct {
        int32_t x, y, z;
    };

    int32_t e[3];
} i3;

typedef union {
    struct {
        int32_t x, y, z, w;
    };

    int32_t e[4];
} i4;

// int64_t vectors
typedef union {
    struct {
        int64_t x, y;
    };

    int64_t e[2];
} l2;

typedef union {
    struct {
        int64_t x, y, z;
    };

    int64_t e[3];
} l3;

typedef union {
    struct {
        int64_t x, y, z, w;
    };

    int64_t e[4];
} l4;

// float vectors
typedef union {
    struct {
        float x, y;
    };

    float e[2];
} f2;

typedef union {
    struct {
        float x, y, z;
    };

    float e[3];
} f3;

typedef union {
    struct {
        float x, y, z, w;
    };

    float e[4];
} f4;

struct mi32 {
    int32_t *e;
    size_t m, n;
};

struct mi64 {
    int64_t *e;
    size_t m, n;
};

struct mf {
    float *e;
    size_t m, n;
};

#endif