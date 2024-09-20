/**
 * Jingga
 *
 * @package   Utils
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PATHFINDING_METRIC2D_H
#define TOS_PATHFINDING_METRIC2D_H

#include <stdio.h>
#include <stdlib.h>

#include "../stdlib/Types.h"
#include "../utils/MathUtils.h"
#include "../memory/RingMemory.h"

float manhattan_2d(v2_f32 a, v2_f32 b) {
    return fabs(a.x - b.x) + fabs(a.y - b.y);
}

float euclidean_2d(v2_f32 a, v2_f32 b) {
    float dx = fabs(a.x - b.x);
    float dy = fabs(a.y - b.y);

    return sqrt(dx * dx + dy * dy);
}

float octile_2d(v2_f32 a, v2_f32 b) {
    float dx = fabs(a.x - b.x);
    float dy = fabs(a.y - b.y);

    return dx < dy
        ? (SQRT_2 - 1) * dx + dy
        : (SQRT_2 - 1) * dy + dx;
}

float chebyshev_2d(v2_f32 a, v2_f32 b) {
    float dx = fabs(a.x - b.x);
    float dy = fabs(a.y - b.y);

    return fmax(dx, dy);
}

float minkowski_2d(v2_f32 a, v2_f32 b, int lambda) {
    float dx = fabs(a.x - b.x);
    float dy = fabs(a.y - b.y);

    return pow(pow(dx, lambda) + pow(dy, lambda), 1.0 / lambda);
}

float canberra_2d(v2_f32 a, v2_f32 b) {
    return (fabs(a.x - b.x) / (fabs(a.x) + fabs(b.x)))
        + (fabs(a.y - b.y) / (fabs(a.y) + fabs(b.y)));
}

float bray_curtis_2d(v2_f32 a, v2_f32 b) {
    return (fabs(a.x - b.x) + fabs(a.y - b.y))
        / ((a.x + b.x) + (a.y + b.y));
}

float angular_separation_2d(v2_f32 a, v2_f32 b) {
    return (a.x * b.x + a.y * b.y)
        / (sqrt(a.x * a.x + a.y * a.y) * sqrt(b.x * b.x + b.y * b.y));
}

#endif