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
#include <math.h>

#define OMS_PI 3.14159265358979323846f
#define OMS_PI_OVER_TWO (OMS_PI / 2.0f)
#define OMS_PI_OVER_FOUR (OMS_PI / 4.0f)
#define OMS_TWO_PI (2 * OMS_PI)

#define OMS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define OMS_MIN(a, b) ((a) > (b) ? (b) : (a))
#define OMS_CLAMP(a, b, c) (OMS_MAX(OMS_MIN((a), (b))), (c))
#define OMS_ABS(a) ((a) > 0 ? (a) : -(a))
#define OMS_DEG2RAD(angle) ((angle) * OMS_PI / 180.0f)
#define OMS_RAD2DEG(angle) ((angle) * 180.0f / OMS_PI)
#define ROUND_TO_NEAREST(a, b) (((a) + ((b) - 1)) & ~((b) - 1))
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

#define SQRT_2 1.4142135623730950488016887242097f

#endif
