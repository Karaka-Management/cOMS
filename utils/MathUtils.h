/**
 * Jingga
 *
 * @package   Utils
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_MATH_UTILS_H
#define TOS_UTILS_MATH_UTILS_H

#include "../stdlib/Intrinsics.h"

#define OMS_PI 3.14159265358979323846f
#define OMS_PI_OVER_TWO OMS_PI / 2.0f
#define OMS_TWO_PI 2 * OMS_PI

#define OMS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define OMS_MIN(a, b) ((a) > (b) ? (b) : (a))
#define OMS_CLAMP(a, b, c) (OMS_MAX(OMS_MIN((a), (b))), (c))
#define OMS_ABS(a) ((a) > 0 ? (a) : -(a))
#define OMS_DEG2RAD(angle) ((angle) * OMS_PI / 180.0f)
#define OMS_RAD2DEG(angle) ((angle) * 180.0f / OMS_PI)
#define ROUND_TO_NEAREST(a, b) (((a) + ((b) - 1)) & ~((b) - 1))

// @question Consider to implement table based sine wave + approximation if necessary
// [-PI/2, PI/2]
inline
float sin_approx_pih_pih(float x)
{
    return x - (x * x * x / 6.0f);
}

inline
float sinf_approx(float x)
{
    x = OMS_RAD2DEG(x);
    return 4 * x * (180 - x) / (40500 - x * (180 - x));
}

inline
float cosf_approx(float x)
{
    return sinf_approx(x + OMS_PI_OVER_TWO);
}

inline
float tanf_approx(float x)
{
    float sin_x = sinf_approx(x);
    float cos_x = cosf_approx(x);

    if (cos_x == 0.0f) {
        return (sin_x > 0.0f) ? 1e10f : -1e10f;
    }

    return sin_x / cos_x;
}

#endif
