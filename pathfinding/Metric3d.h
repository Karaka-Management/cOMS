/**
 * Jingga
 *
 * @package   Utils
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PATHFINDING_METRIC3D_H
#define TOS_PATHFINDING_METRIC3D_H

#include <stdio.h>
#include <stdlib.h>

#include "../stdlib/Mathtypes.h"
#include "../memory/RingMemory.h"

// Manhattan distance for 3D
float manhattan_3d(v3_f32 a, v3_f32 b) {
    return fabs(a.x - b.x)
        + fabs(a.y - b.y)
        + fabs(a.z - b.z);
}

// Euclidean distance for 3D
float euclidean_3d(v3_f32 a, v3_f32 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

// Chebyshev distance for 3D
float chebyshev_3d(v3_f32 a, v3_f32 b) {
    float dx = fabs(a.x - b.x);
    float dy = fabs(a.y - b.y);
    float dz = fabs(a.z - b.z);

    return fmax(fmax(dx, dy), dz);
}

// Minkowski distance for 3D
float minkowski_3d(v3_f32 a, v3_f32 b, int lambda) {
    float dx = fabs(a.x - b.x);
    float dy = fabs(a.y - b.y);
    float dz = fabs(a.z - b.z);

    return pow(
        pow(dx, lambda)
        + pow(dy, lambda)
        + pow(dz, lambda),
        1.0 / lambda
    );
}

// Canberra distance for 3D
float canberra_3d(v3_f32 a, v3_f32 b) {
    return fabs(a.x - b.x) / (fabs(a.x) + fabs(b.x))
        + fabs(a.y - b.y) / (fabs(a.y) + fabs(b.y))
        + fabs(a.z - b.z) / (fabs(a.z) + fabs(b.z));
}

// Bray-Curtis distance for 3D
float bray_curtis_3d(v3_f32 a, v3_f32 b) {
    return (fabs(a.x - b.x) + fabs(a.y - b.y) + fabs(a.z - b.z))
        / ((a.x + b.x) + (a.y + b.y) + (a.z + b.z));
}

// Angular separation for 3D
float angular_separation_3d(v3_f32 a, v3_f32 b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z)
        / (sqrt(a.x * a.x + a.y * a.y + a.z * a.z) * sqrt(b.x * b.x + b.y * b.y + b.z * b.z));
}

// Hamming distance for arrays
int hamming(int* a, int* b, int size) {
    int dist = 0;

    for (int i = 0; i < size; ++i) {
        if (a[i] != b[i]) {
            ++dist;
        }
    }

    return dist;
}

#endif