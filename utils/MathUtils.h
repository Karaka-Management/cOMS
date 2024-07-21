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
#define OMS_PI_OVER_FOUR OMS_PI / 4.0f
#define OMS_TWO_PI 2 * OMS_PI

#define OMS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define OMS_MIN(a, b) ((a) > (b) ? (b) : (a))
#define OMS_CLAMP(a, b, c) (OMS_MAX(OMS_MIN((a), (b))), (c))
#define OMS_ABS(a) ((a) > 0 ? (a) : -(a))
#define OMS_DEG2RAD(angle) ((angle) * OMS_PI / 180.0f)
#define OMS_RAD2DEG(angle) ((angle) * 180.0f / OMS_PI)
#define ROUND_TO_NEAREST(a, b) (((a) + ((b) - 1)) & ~((b) - 1))

#ifndef FLT_MIN
    #define FLT_MIN 1.175494e-038
#endif

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

inline
float atanf_approx(float x)
{
    float abs_x = OMS_ABS(x);
    float result;

    if (abs_x > 1.0f) {
        result = OMS_PI_OVER_TWO - (1.0f / abs_x);
    } else {
        result = abs_x - (abs_x * abs_x * abs_x / 3.0f);
    }

    return (x < 0.0f) ? -result : result;
}

inline
float atan2f_approx(float y, float x)
{
    float abs_y = OMS_ABS(y) + FLT_MIN; // prevent division by zero
    float angle;

    if (x >= 0.0f) {
        float r = (x - abs_y) / (x + abs_y);
        angle = OMS_PI_OVER_FOUR - OMS_PI_OVER_FOUR * r;
    } else {
        float r = (x + abs_y) / (abs_y - x);
        angle = (3.0f * OMS_PI / 4.0f) - OMS_PI_OVER_FOUR * r;
    }

    return (y < 0.0f) ? -angle : angle;
}

inline
float asinf_approx(float x)
{
    float negate = (x < 0) ? 1.0f : 0.0f;
    x = OMS_ABS(x);

    float result = -0.0187293f;
    result *= x;
    result += 0.0742610f;
    result *= x;
    result -= 0.2121144f;
    result *= x;
    result += 1.5707288f;
    result *= sqrt(1.0f - x);
    result -= 2 * negate * result;

    return negate * OMS_PI + result;
}

#endif
