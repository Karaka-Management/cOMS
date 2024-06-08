/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_MATH_UTILS_H
#define UTILS_MATH_UTILS_H

#include "../Stdlib/Intrinsics.h"

#define OMS_PI 3.14159265358979323846f
#define OMS_PI_OVER_TWO OMS_PI / 2.0f
#define OMS_TWO_PI 2 * OMS_PI

#define OMS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define OMS_MIN(a, b) ((a) > (b) ? (b) : (a))
#define OMS_ABS(a) ((a) > 0 ? (a) : -(a))
#define OMS_DEG2RAD(angle) ((angle) * OMS_PI / 180.0f)
#define OMS_RAD2DEG(angle) ((angle) * 180.0f / OMS_PI)
#define ROUND_TO_NEAREST(a, b) (((a) + ((b) - 1)) & ~((b) - 1))

// @todo implement table based sine wave + approximation if necessary
// [-PI/2, PI/2]
inline
float sin_approx_pih_pih(float x)
{
    return x - (x * x * x / 6.0f);
}

inline
float sinf_approx(float x)
{
    x = x - OMS_TWO_PI * Stdlib::Intrinsics::floor((x + OMS_PI) / OMS_TWO_PI);

    if (x > OMS_PI_OVER_TWO) {
        x = OMS_PI - x;
    } else if (x < -OMS_PI_OVER_TWO) {
        x = -OMS_PI - x;
    }

    return x - (x * x * x / 6.0f);
}

#endif
