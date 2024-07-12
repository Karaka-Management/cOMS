/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_H
#define TOS_UTILS_H

#include "../stdlib/Types.h"

struct file_body {
    uint64 size = 0; // doesn't include null termination (same as strlen)
    byte* content;
};

global_persist uint32 fast_seed;
#define FAST_RAND_MAX 32767

inline
uint32 fast_rand(void) {
    fast_seed = (214013 * fast_seed + 2531011);

    return (fast_seed >> 16) & 0x7FFF;
}

inline
f32 fast_rand_percentage(void) {
    return (f32) fast_rand() / (f32) FAST_RAND_MAX;
}

inline
bool is_bit_set(byte data, byte bit)
{
    return (data & (1 << bit)) == 0;
}

void make_character(
    float *data,
    float x, float y, float n, float m, char c)
{
    float *d = data;

    // Texture atlas is 16 characters
    // 1 / 16 = 0.0625
    float a = 0.0625;
    float b = 0.0625 * 2;

    // ascii offset
    int w = c - 32;

    float du = (w % 16) * a;
    float dv = 1 - (w / 16) * b - b;

    // Quad data (2 triangles)
    *(d++) = x - n; *(d++) = y - m;
    *(d++) = du + 0; *(d++) = dv;
    *(d++) = x + n; *(d++) = y - m;
    *(d++) = du + a; *(d++) = dv;
    *(d++) = x + n; *(d++) = y + m;
    *(d++) = du + a; *(d++) = dv + b;
    *(d++) = x - n; *(d++) = y - m;
    *(d++) = du + 0; *(d++) = dv;
    *(d++) = x + n; *(d++) = y + m;
    *(d++) = du + a; *(d++) = dv + b;
    *(d++) = x - n; *(d++) = y + m;
    *(d++) = du + 0; *(d++) = dv + b;
}

#endif